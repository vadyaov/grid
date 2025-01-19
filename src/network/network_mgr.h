#pragma once

#include "network.h"

#include <memory>

struct TrainingConfig {
    std::string dataPath;
    int epochs;
    int miniBatchSize;
    double eta;
};

class NetworkMgr {
public:
    NetworkMgr(std::vector<int>&& config);
    int recognize(std::vector<double>& input);
    void train(const TrainingConfig& config);

private:
    std::unique_ptr<Network> net_;
};