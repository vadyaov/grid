#pragma once

#include <vector>

#include <Eigen/Dense>

using Eigen::MatrixXd;

class Network {
public:
    Network(std::vector<int>&& layers);

    MatrixXd feedForward(MatrixXd a) const;

    void dump() const noexcept;
private:
    int layers_;                                     // number of layers
    std::vector<int> sizes_;                         // number of neurons in layers
    std::vector<MatrixXd> biases_;  // biases for layers [1, sizes)
    std::vector<MatrixXd> weights_; // weights between layers
};