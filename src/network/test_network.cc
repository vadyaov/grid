#include "network.h"
#include "../loader/loader.h"

#include <iostream>

int main() {
    Network net({784, 30, 10});
    auto dataset = MNISTLoader::LoadData("/home/vadim/Projects/mlp/data/");
    net.SGD(dataset.training_data, 30, 10, 10.0, &dataset.test_data);
    return 0;
}