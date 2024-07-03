#include "linear.h"

LinearRegression::LinearRegression(uint64_t D){
    d = D;
    weights = zeros(d,1);
    weights_ = zeros(d,1);
    bias = 0;
    epsilon = EPS;
    eta = ETA;
}

double LinearRegression::l2loss(matrix X, matrix Y){
   // cout<<"l2Loss"<<endl;
    matrix Y_pred = matmul(X,weights) + bias;
    __size d1 = Y.shape(), d2 = Y_pred.shape();
    if (d1 != d2){
        throw std::invalid_argument("Cannot compute loss of vectors with dimensions ( "+to_string(d1.first)+" , "
        +to_string(d1.second)+" ) and ( "+to_string(d2.first)+" , "+to_string(d2.second)+" ) do not match");
    }
    // Compute the mean squared loss as defined in README.md
    double loss = 0;
    matrix temp_loss(Y.rows,Y.cols);
    temp_loss = Y_pred-Y;
    loss = norm(temp_loss);
    loss = loss*loss;
    loss/=(Y.rows);
    return loss;
}

pair<matrix, double> LinearRegression::l2lossDerivative(matrix X, matrix Y){
    //cout<<"l2LossDerivative"<<endl;
    //cout<<X.rows<<" "<<X.cols<<" "<<Y.rows<<" "<<Y.cols<<endl;
    matrix Y_pred = matmul(X,weights) + bias;
    __size d1 = Y.shape(), d2 = Y_pred.shape();
    if (d1 != d2){
        throw std::invalid_argument("Cannot compute loss derivative of vectors with dimensions ( "+to_string(d1.first)+" , "
        +to_string(d1.second)+" ) and ( "+to_string(d2.first)+" , "+to_string(d2.second)+" ) do not match");
    }
    //Compute gradients as defined in README.md
    matrix dw(d,1);
    double db=0; 
    matrix Z(Y.rows,Y.cols);
    Z = Y_pred -Y;
    uint64_t n= X.rows;
    matrix temp(X.cols,X.rows);
    temp = X.transpose();
    dw = matmul(temp,Z);
    dw= dw/n;
    dw = dw*2;
    for(uint64_t x=0; x<n;x++){
        db+=Z.data[x];
    }
    db/=n;
    db*=2;
    return {dw,db};
}

matrix LinearRegression::predict(matrix X){
    //cout<<"predict"<<endl;
    matrix Y_pred(X.shape().first,1);
    // Using the weights and bias, find the values of y for every x in X
    Y_pred = matmul(X,weights) + bias;
    return Y_pred;
}  

void LinearRegression::GD(matrix X, matrix Y,double learning_rate, uint64_t limit){
   // cout<<"GD"<<endl;
    eta = learning_rate;
    double old_loss = 0,loss = l2loss(X,Y);
    train_loss.PB(loss);
    uint64_t iteration = 0;
    max_iterations = limit;
    while (fabs(loss - old_loss) > epsilon && iteration < max_iterations){
        // Calculate the gradients and update the weights and bias correctly. Do not edit anything else 
        
        pair<matrix,double> temp = l2lossDerivative(X,Y);
        //cout<<"DW"<<endl;
        //temp.first.printMatrix();
        //cout<<"DB"<<" "<<temp.second<<endl;
        //cout<<"Weights"<<endl;
        //weights.printMatrix();
        //cout<<"Bias"<<endl;
        //cout<<bias<<endl;
        //cout<<"=============="<<endl;
        weights = weights - eta*temp.first;
        bias = bias - eta*temp.second;
        old_loss=loss;
        loss =  l2loss(X,Y);
        if (iteration %100 == 0) train_loss.PB(loss);
        iteration++;
    }
}

void LinearRegression::train(matrix X,matrix Y,double learning_rate, uint64_t limit){
    /* 
     * These lines are for computing the closed form solution of weights in the case of zero bias. 
     * This is merely a reference and you do not need to try and match the predictions from the trained weights with 
     * those from weight_. 
     * However you do need to get a good enough accuracy and that is obtained by varying the learning rate and 
     * maximum number of iterations .
    */
    matrix Xt = X.transpose();
    matrix Z = matmul(Xt,X);
    GD(X,Y,learning_rate,limit);
    weights_ = matmul((Z.inverse()),matmul(Xt,Y));
    cout << "Training Loss\n";
    for (uint64_t i = 0; i < train_loss.size() ; i++){
        cout << train_loss[i] << "\n";
    }
}

void LinearRegression::test(matrix X,matrix Y){
    matrix Y_pred = predict(X);
    uint64_t n = X.shape().first;
    matrix Y_closed = matmul(X,weights_);
    cout << "Predictions(GD) \t Predictions(C) \t True Value\n"; 
    for (uint64_t i = 0 ; i < n ; i++){
        cout << Y_pred(i,0) << "\t\t\t "<< Y_closed(i,0) << "\t\t\t " << Y(i,0) << "\n";
    }
    cout << "Testing loss " << l2loss(X,Y) << "\n";
    cout << "Testing accuracy " << accuracy(Y_pred,Y) << "\n";
}

double LinearRegression::accuracy(matrix Y_pred, matrix Y){
    //cout<<"Accuracy"<<endl;
    double acc = 0; 
    // Compute the accuracy of the model
    matrix diff(Y.rows,Y.cols);
    diff = Y_pred -Y;
    diff = fabs(diff);
    matrix temp(Y.rows,Y.cols);
    temp = fabs(Y);
    diff = diff/temp;
    diff = diff/(Y.rows);
    for(uint64_t z=0; z<Y.rows;z++){
        //cout<<"=========="<<endl;
        //cout<<Y_pred.data[z]<<" "<<Y.data[z]<<endl;
    acc+=diff.data[z];
    //cout<<diff.data[z]<<endl;
    }
    acc = 1-acc;
    return acc;
}

pair<pair<matrix, matrix>, pair<matrix, matrix>> test_train_split(matrix X, matrix Y, float ratio) {
    if (ratio < 0 || ratio > 1) {
        throw std::invalid_argument("Ratio must be between 0 and 1");
    }

    uint64_t n = X.shape().first;
    uint64_t train_size = static_cast<uint64_t>(n * ratio);
    uint64_t test_size = n - train_size;

    std::vector<uint64_t> indices(n);
    std::iota(indices.begin(), indices.end(), 0);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);

    matrix X_train(train_size, X.shape().second);
    matrix Y_train(train_size, 1);
    matrix X_test(test_size, X.shape().second);
    matrix Y_test(test_size, 1);

    for (uint64_t i = 0; i < train_size; ++i) {
        for (uint64_t j = 0; j < X.shape().second; ++j) {
            X_train(i, j) = X(indices[i], j);
        }
        Y_train(i, 0) = Y(indices[i], 0);
    }

    for (uint64_t i = 0; i < test_size; ++i) {
        for (uint64_t j = 0; j < X.shape().second; ++j) {
            X_test(i, j) = X(indices[train_size + i], j);
        }
        Y_test(i, 0) = Y(indices[train_size + i], 0);
    }

    return {{X_train, Y_train}, {X_test, Y_test}};
}