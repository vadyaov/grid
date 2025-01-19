#include "network_mgr.h"
#include "../loader/loader.h"

#include <iostream>

NetworkMgr::NetworkMgr(std::vector<int>&& config) :
    net_{new Network(std::move(config))} {}

int NetworkMgr::recognize(std::vector<double>& input) {
  Eigen::Map<Eigen::VectorXd> inp(input.data(), input.size());
  Eigen::MatrixXd output = net_->FeedForward(inp);
  if (output.cols() != 1) {
    std::cerr << "COlS != 1. BAD" << std::endl;
  }
  std::cout << output << std::endl;

  Eigen::Index max_row, max_col;
  std::cout << "max_idx: " << output.maxCoeff(&max_row, &max_col) << std::endl;
  std::cout << "row: " << max_row << std::endl;
  std::cout << "col: " << max_col << std::endl;

  return max_row; 
}

void NetworkMgr::train(const TrainingConfig& config) {
  if (net_->isTrained()) {
    std::cout << "Using cached training." << std::endl;
    return;
  }

  auto dataset = MNISTLoader::LoadData(config.dataPath);
  for (int i = 0; i < 10; ++i) {
    std::cout << dataset.test_data[i].first.reshaped(28, 28).transpose() << "\n" << std::endl;
  }
  net_->SGD(dataset.training_data, config.epochs,
            config.miniBatchSize, config.eta, &dataset.test_data);
}