g++ testBasicOperators.cpp ../matrix/naive.cpp -o basicNaive
g++ testExp.cpp ../matrix/naive.cpp -o expNaive
g++ testMatrOps.cpp ../matrix/naive.cpp -o matrOpsNaive
g++ testMinMax.cpp ../matrix/naive.cpp -o minMaxNaive
g++ -mavx testBasicOperators.cpp ../matrix/optim.cpp -o basicOptim # feel free to edit
g++ -mavx testExp.cpp ../matrix/optim.cpp -o expOptim # feel free to edit
g++ -mavx testMatrOps.cpp ../matrix/optim.cpp -o matrOpsOptim # feel free to edit
g++ -mavx testMinMax.cpp ../matrix/optim.cpp -o minMaxOptim # feel free to edit
