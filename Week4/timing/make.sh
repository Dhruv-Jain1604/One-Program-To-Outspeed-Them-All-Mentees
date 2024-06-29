g++ timeBasicOperators.cpp ../matrix/naive.cpp -o basicNaive
g++ timeExp.cpp ../matrix/naive.cpp -o expNaive
g++ timeMatrOps.cpp ../matrix/naive.cpp -o matrOpsNaive
g++ timeMinMax.cpp ../matrix/naive.cpp -o minMaxNaive
g++ -mavx timeBasicOperators.cpp ../matrix/optim.cpp  -o basicOptim # feel free to edit
g++  -mavx timeExp.cpp ../matrix/optim.cpp -o expOptim # feel free to edit
g++ -mavx timeMatrOps.cpp ../matrix/optim.cpp -o matrOpsOptim # feel free to edit
g++ -mavx timeMinMax.cpp ../matrix/optim.cpp -o minMaxOptim # feel free to edit
