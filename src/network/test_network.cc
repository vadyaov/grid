#include "network.h"

#include <iostream>

int main() {
    Network net({2, 3, 1});
    net.dump();

    MatrixXd inputMatrix(2, 1);
    inputMatrix(0, 0) = 3;
    inputMatrix(1, 0) = 2;
    std::cout << net.FeedForward(inputMatrix);
    return 0;
}