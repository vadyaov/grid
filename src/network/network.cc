#include "network.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <fstream>

static const char* CACHED_TRAINING_PATH = "/home/vadim/Projects/mlp/src/cached_training";

static inline double RandomGaussValue() {
  static std::random_device rd{};
  static std::mt19937 gen{rd()};
  static std::normal_distribution<double> normal{0, 1};

  return normal(gen);
}

MatrixXd Network::Sigmoid(const MatrixXd &z) {
  return (1.0 / (1.0 + (-z).array().exp())).matrix();
}

MatrixXd Network::SigmoidPrime(const MatrixXd &z) {
  return (Sigmoid(z).array() * (1 - Sigmoid(z).array())).matrix();
}

MatrixXd Network::CostDeriative(const MatrixXd& output_actiavtions, const MatrixXd& y) {
  return output_actiavtions - y;
}

bool Network::LoadTraining(const char* filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file) return false;

  try {
    // Read number of layers
    int layers;
    file.read(reinterpret_cast<char*>(&layers), sizeof(layers));
    if (layers <= 0) return false;
    layers_ = layers;

    // Read number of neurons in layers
    sizes_.clear();
    sizes_.reserve(layers);
    for (int i = 0; i < layers; ++i) {
      int size;
      file.read(reinterpret_cast<char*>(&size), sizeof(size));
      if (size <= 0) return false;
      sizes_.push_back(size);
    }

    // Read biases
    biases_.clear();
    biases_.reserve(layers - 1);
    for (int i = 1; i < layers_; ++i) {
      Eigen::Index rows, cols;
      file.read(reinterpret_cast<char*>(&rows), sizeof(rows));
      file.read(reinterpret_cast<char*>(&cols), sizeof(cols));

      MatrixXd matrix(rows, cols);
      file.read(reinterpret_cast<char*>(matrix.data()), rows * cols * sizeof(double));
      biases_.push_back(std::move(matrix));
    }

    // Read weights
    weights_.clear();
    weights_.reserve(layers - 1);
    for (int i = 1; i < layers_; ++i) {
      Eigen::Index rows, cols;
      file.read(reinterpret_cast<char*>(&rows), sizeof(rows));
      file.read(reinterpret_cast<char*>(&cols), sizeof(cols));

      MatrixXd matrix(rows, cols);
      file.read(reinterpret_cast<char*>(matrix.data()), rows * cols * sizeof(double));
      weights_.push_back(std::move(matrix));
    }
  } catch (const std::exception& /* exception */) {
    return false;
  }

  return true;
}

void Network::CacheParameters(const char* filename) {
  std::ofstream file(filename, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Cannot create cache file");
  }

  file.write(reinterpret_cast<const char*>(&layers_), sizeof(layers_));
  for (int size : sizes_) {
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
  }

  for (const auto& matrix : biases_) {
    Eigen::Index rows = matrix.rows();
    Eigen::Index cols = matrix.cols();
    file.write(reinterpret_cast<const char*>(&rows), sizeof(rows));
    file.write(reinterpret_cast<const char*>(&cols), sizeof(cols));
    file.write(reinterpret_cast<const char*>(matrix.data()), rows * cols * sizeof(double));
  }

  for (const auto& matrix : weights_) {
    Eigen::Index rows = matrix.rows();
    Eigen::Index cols = matrix.cols();
    file.write(reinterpret_cast<const char*>(&rows), sizeof(rows));
    file.write(reinterpret_cast<const char*>(&cols), sizeof(cols));
    file.write(reinterpret_cast<const char*>(matrix.data()), rows * cols * sizeof(double));
  }
}

Network::Network(std::vector<int> &&sizes) : sizes_{std::move(sizes)} {
  // TODO: add cache versions + validate data after loading from cache
  if (LoadTraining(CACHED_TRAINING_PATH)) {
    isTrained = true;
    return;
  }

  // TODO: InitializeNetwork() private method
  // TODO: InitializeNetwork() private method
  // TODO: InitializeNetwork() private method
  layers_ = static_cast<int>(sizes_.size());
  biases_.reserve(layers_ - 1);
  weights_.reserve(layers_ - 1);
  for (auto layer_number = 1; layer_number < layers_; ++layer_number) {
    MatrixXd biasMatrix(sizes_[layer_number], 1);
    biasMatrix =
        biasMatrix.unaryExpr([&](double dummy) { return RandomGaussValue(); });
    biases_.emplace_back(std::move(biasMatrix));

    MatrixXd weightMatrix(sizes_[layer_number], sizes_[layer_number - 1]);
    weightMatrix = weightMatrix.unaryExpr(
        [&](double dummy) { return RandomGaussValue(); });
    weights_.emplace_back(std::move(weightMatrix));
  }
}

MatrixXd Network::FeedForward(MatrixXd a) const {
  for (size_t i = 0; i < biases_.size(); ++i) {
    a = Sigmoid(weights_[i] * a + biases_[i]);
  }
  return a;
}

void Network::SGD(std::vector<VectorXdPair> &training_data, size_t epochs,
                  size_t mini_batch_size, double eta,
                  const std::vector<VectorXdPair> *test_data) {
  std::cout << "Start training..." << std::endl;
  if (isTrained) return;

  int n_test = test_data ? test_data->size() : 0;
  int n      = training_data.size();

  std::random_device rd;
  std::mt19937 gen(rd());

  for (int j = 0; j < epochs; ++j) {
    std::shuffle(training_data.begin(), training_data.end(), gen);
    std::vector<std::vector<VectorXdPair>> mini_batches;
    // (n + k - 1) / k -- всегда выделим достаточно места, даже если не делится нацело
    mini_batches.reserve((n + mini_batch_size - 1) / mini_batch_size);
    for (int k = 0; k < n; k += mini_batch_size) {
      size_t batch_size = std::min(mini_batch_size, training_data.size() - k);
      mini_batches.emplace_back(training_data.begin() + k,
                                training_data.begin() + k + batch_size);
    }

    for (const auto& mini_batch : mini_batches) {
      UpdateMiniBatch(mini_batch, eta);
    }

    if (test_data) {
      std::cout << "Epoch " << j << ": "
                << Evaluate(*test_data) << " / "
                << n_test << std::endl;
    } else {
      std::cout << "Epoch " << j << " complete" << std::endl;
    }
  }
  try {
    CacheParameters(CACHED_TRAINING_PATH);
  } catch(const std::exception& e) {
    std::cerr << "Failed to cache parameters: " << e.what() << std::endl;
  }
  std::cout << "End training." << std::endl;
}

void Network::UpdateMiniBatch(const std::vector<VectorXdPair> &mini_batch, double eta) {
  std::vector<MatrixXd> nabla_b; // for biases
  std::vector<MatrixXd> nabla_w; // for weights

  nabla_b.reserve(biases_.size());
  nabla_w.reserve(weights_.size());

  for (const auto& b : biases_) {
    nabla_b.emplace_back(MatrixXd::Zero(b.rows(), b.cols()));
  }

  for (const auto& w : weights_) {
    nabla_w.emplace_back(MatrixXd::Zero(w.rows(), w.cols()));
  }

  for (const auto& [x, y] : mini_batch) {
    // std::cout << "nabla_b.size() = " << nabla_b.size() << std::endl;
    // std::cout << "nabla_w.size() = " << nabla_w.size() << std::endl;

    // Градиент функции стоимости
    auto [delta_nabla_b, delta_nabla_w] = Backprop(x, y);
    for (size_t i = 0; i < nabla_b.size(); ++i) {
      nabla_b[i] += delta_nabla_b[i];
      nabla_w[i] += delta_nabla_w[i];
    }
  }

  // std::cout << "weights_.size() = " << weights_.size() << std::endl;
  // std::cout << "biases_.size() = " << biases_.size() << std::endl;

  double learning_rate = eta / mini_batch.size();
  for (size_t i = 0; i < weights_.size(); ++i) {
    weights_[i] -= learning_rate * nabla_w[i];
    biases_[i] -= learning_rate * nabla_b[i];
  }
}

std::pair<std::vector<MatrixXd>, std::vector<MatrixXd>>
Network::Backprop(const Eigen::VectorXd& x, const Eigen::VectorXd& y) {
  std::vector<MatrixXd> nabla_b;
  std::vector<MatrixXd> nabla_w;

  for (const auto& b : biases_) {
    nabla_b.emplace_back(MatrixXd::Zero(b.rows(), b.cols()));
  }

  for (const auto& w: weights_) {
    nabla_w.emplace_back(MatrixXd::Zero(w.rows(), w.cols()));
  }

  MatrixXd activation = x.matrix();
  std::vector<MatrixXd> activations {activation};
  std::vector<MatrixXd> zs;

  // Прямой проход
  for (size_t i = 0; i < biases_.size(); ++i) {
    MatrixXd z = weights_[i] * activation + biases_[i];
    zs.push_back(z);
    activation = Sigmoid(z);
    activations.push_back(activation);
  }

  // Обратный проход
  MatrixXd delta = CostDeriative(activations.back(), y).array() *
                   SigmoidPrime(zs.back()).array();
  
  nabla_b.back() = delta;
  nabla_w.back() = delta * activations[activations.size() - 2].transpose();

  for (int l = 2; l < layers_; ++l) {
    const MatrixXd& z = zs[zs.size() - l];
    MatrixXd sp = SigmoidPrime(z);
    delta = (weights_[weights_.size() - l + 1].transpose() * delta).array() * sp.array();
    nabla_b[nabla_b.size() - l] = delta;
    nabla_w[nabla_w.size() - l] = delta *
        activations[activations.size() - l - 1].transpose();
  }

  return {nabla_b, nabla_w};
}

int Network::Evaluate(const std::vector<VectorXdPair>& test_data) {
  int test_result = 0;
  for (const auto& [x, y] : test_data) {
    auto output = FeedForward(x);

    Eigen::Index predicated_row, predicated_col;
    output.maxCoeff(&predicated_row, &predicated_col);

    Eigen::Index actual_row, actual_col;
    y.maxCoeff(&actual_row, &actual_col);

    // Достаточно сравнить только row, потому что матрица всегда размера (rows, 1)
    if (predicated_row == actual_row && predicated_col == actual_col) ++test_result;
  }
  return test_result;
}

void Network::dump() const noexcept {
  std::cout << "Biases:\n";
  int i = 0;
  for (auto &m : biases_) {
    std::cout << "[" << i++ << "]\n" << m << std::endl;
  }
  i = 0;
  std::cout << "Weights:\n";
  for (auto &m : weights_) {
    std::cout << "[" << i++ << "]\n" << m << std::endl;
  }
}