#include "logistic.h"
#include "../matrix/matrix.h"

LogisticRegression::LogisticRegression(uint64_t D){
    d = D;
    weights = zeros(d,1);
    bias = 0;
    epsilon = EPS;
    eta = ETA;
}

matrix LogisticRegression::sigmoid(matrix z){
    matrix g = 1.0*ones(z.rows,1);
    // Implement sigmoid function here as defined in README.md  
    //cout<<"entered sigmoid"<<endl;
    //z.printMatrix();
    //cout<<"-====================="<<endl;
    matrix n_z(z.rows,z.cols);
    n_z = -z;
    //n_z.printMatrix();
    //cout<<"====================="<<endl;
    g = g + exp(n_z);
    //g.printMatrix();
    //cout<<"======================"<<endl;
    g = 1/g; 
    //g.printMatrix(); 
    //cout<<"-------------------------"<<endl;
    return g;
}

double LogisticRegression::logisticLoss(matrix X, matrix Y){
    matrix Y_pred = sigmoid((matmul(X,weights) + bias));
    __size d1 = Y.shape(), d2 = Y_pred.shape();
    if (d1 != d2){
        throw std::invalid_argument("Cannot compute loss of vectors with dimensions ( "+to_string(d1.first)+" , "
        +to_string(d1.second)+" ) and ( "+to_string(d2.first)+" , "+to_string(d2.second)+" ) do not match");
    }
    double loss = 0;
    // Compute the log loss as defined in README.md
    //Y.printMatrix();
    //cout<<"----------"<<endl;
    //Y_pred.printMatrix();
    //cout<<"--------------------"<<endl;
    matrix m1(Y.rows,Y.cols);
    matrix m2(Y.rows,Y.cols);
    matrix m3(Y.rows,Y.cols);
    matrix m4(Y.rows,Y.cols);
    matrix m5(Y.rows,Y.cols);
    matrix m6(Y.rows,Y.cols);
    matrix m8(Y.rows,Y.cols);
    matrix m7(Y.rows,Y.cols);
    double buffer = 1e-4;
    uint64_t n = Y.rows;
    m1 = -Y;
    m8 = Y_pred +buffer;
    //cout<<"printing matirx"<<endl;
    //m8.printMatrix();
    m2 = log(m8);
    //cout<<"Printing m2"<<endl;
    //m2.printMatrix();
    m3 = Y-1;
    m7 = -Y_pred;
    m4 = m7 +(1+buffer);
    //cout<<"printing m4"<<endl;
    //cout<<"--------"<<endl;
    m5 = log(m4);
    m6 = m1*m2 + m3*m5;
    //cout<<"------------------------"<<endl;
    for(uint64_t a=0;a<n;a++){
        loss+=m6.data[a];
        //cout<<m6.data[a]<<endl;
    }
    //cout<<"-------------------"<<endl;
    loss = loss/n;
    return loss;
}

pair<matrix, double> LogisticRegression::lossDerivative(matrix X, matrix Y){
    matrix Y_pred = sigmoid((matmul(X,weights) + bias));
    __size d1 = Y.shape(), d2 = Y_pred.shape();
    if (d1 != d2){
        throw std::invalid_argument("Cannot compute loss derivative of vectors with dimensions ( "+to_string(d1.first)+" , "
        +to_string(d1.second)+" ) and ( "+to_string(d2.first)+" , "+to_string(d2.second)+" ) do not match");
    }
    //Compute gradients as defined in README.md
    matrix dw(d,1);
    double db=0;
    matrix X_t(X.cols,X.rows);
    matrix diff(Y.rows,Y.cols);
    uint64_t n = Y.rows;
    X_t = X.transpose();
    diff = Y_pred-Y;
    dw = matmul(X_t,diff);
    dw = dw/n;
    for(uint64_t counter=0;counter<n;counter++){
        db+=diff.data[counter];
    }
    db/=n;
    //dw.printMatrix();
    //cout<<db<<endl;
    //cout<<"--------------"<<endl;
    return {dw,db};
}

matrix LogisticRegression::predict(matrix X){
    matrix Y_pred(X.shape().first,1);
    // Using the weights and bias, find the values of y for every x in X
    Y_pred = sigmoid((matmul(X,weights) + bias));
    uint64_t rows_number = Y_pred.rows;
    for(uint64_t z=0;z<rows_number;z++){
        if(Y_pred.data[z]>=0.5) Y_pred.data[z]=1;
        else Y_pred.data[z]=0;
    }
    return Y_pred;
}  

void LogisticRegression::GD(matrix X, matrix Y,double learning_rate, uint64_t limit){
    eta = learning_rate;
    double old_loss = 0,loss = logisticLoss(X,Y);
    //cout<<loss<<" "<<old_loss<<endl;
    //cout<<"=============="<<endl;
    train_loss.PB(loss);
    uint64_t iteration = 0;
    max_iterations = limit;
    while (fabs(loss - old_loss) > epsilon && iteration < max_iterations){
        // Calculate the gradients and update the weights and bias correctly. Do not edit anything else 
        pair<matrix,double> temp = lossDerivative(X,Y);
        //cout<<"Old Weights"<<endl;
        //weights.printMatrix();
        //cout<<"====================="<<endl;
        weights = weights - eta*(temp.first);
        //cout<<"new weights"<<endl;
        //weights.printMatrix();
        //cout<<"======================="<<endl;
        bias = bias - eta*(temp.second);
        old_loss= loss;
        loss = logisticLoss(X,Y);
        //cout<<loss<<" "<<old_loss<<endl;
        //cout<<"=========="<<iteration<<"-------------"<<endl;
        if (iteration %100 == 0) train_loss.PB(loss);
        iteration++;
    }
}

void LogisticRegression::train(matrix X,matrix Y,double learning_rate, uint64_t limit){
    GD(X,Y,learning_rate,limit);
    cout << "Training Loss\n";
    for (uint64_t i = 0; i < train_loss.size() ; i++){
        cout << train_loss[i] << "\n";
    }
}

void LogisticRegression::test(matrix X,matrix Y){
    matrix Y_pred = predict(X);
    uint64_t n = X.shape().first;
    cout << "Predictions(GD) \t True Value\n"; 
    for (uint64_t i = 0 ; i < n ; i++){
        cout << Y_pred(i,0) <<  "\t\t\t " << Y(i,0) << "\n";
    }
    cout << "Testing loss " << logisticLoss(X,Y) << "\n";
    cout << "Testing accuracy " << accuracy(Y_pred,Y) << "\n";
}

double LogisticRegression::accuracy(matrix Y_pred, matrix Y){
    double acc = 0;
    // Compute the accuracy of the model
    uint64_t size = Y.rows;
    for(uint64_t check=0;check<size;check++){
        if(Y.data[check]==Y_pred.data[check]){
            acc++;
        }
    }
    acc= acc/size;
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