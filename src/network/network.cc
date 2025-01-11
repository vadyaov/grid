#include "network.h"
#include <iostream>
#include <random>

static inline double RandomGaussValue() {
    static std::random_device rd{};
    static std::mt19937 gen{rd()};
    static std::normal_distribution<double> normal{0, 1};

    return normal(gen);
}

static inline MatrixXd sigmoid(const MatrixXd& z) {
    return (1.0 / (1.0 + (-z).array().exp())).matrix();
}

Network::Network(std::vector<int>&& sizes) : sizes_{std::move(sizes)} {
    layers_ = static_cast<int>(sizes_.size());
    for (auto layer_number = 1; layer_number < layers_; ++layer_number) {
        MatrixXd biasMatrix(sizes_[layer_number], 1);
        biasMatrix = biasMatrix.unaryExpr([&](double dummy) { return RandomGaussValue(); });
        biases_.emplace_back(std::move(biasMatrix));

        MatrixXd weightMatrix(sizes_[layer_number], sizes_[layer_number - 1]);
        weightMatrix = weightMatrix.unaryExpr([&](double dummy) { return RandomGaussValue(); });
        weights_.emplace_back(std::move(weightMatrix));
    }
}

MatrixXd Network::feedForward(MatrixXd a) const {
    for (size_t i = 0; i < biases_.size(); ++i) {
        a = sigmoid(weights_[i] * a + biases_[i]);
    }
    return a;
}

void Network::dump() const noexcept {
    std::cout << "Biases:\n";
    int i = 0;
    for (auto& m : biases_) {
        std::cout << "[" << i++ << "]\n" << m << std::endl;
    }
    i = 0;
    std::cout << "Weights:\n";
    for (auto& m : weights_) {
        std::cout << "[" << i++ << "]\n" << m << std::endl;
    }
}