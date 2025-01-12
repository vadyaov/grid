#include "network.h"

#include <iostream>

int main() {
    Network net({784, 30, 10});

    MatrixXd inputMatrix = MatrixXd::Random(784, 1);
    std::cout << net.FeedForward(inputMatrix);
    return 0;
}