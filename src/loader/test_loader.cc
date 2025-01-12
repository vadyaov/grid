#include "loader.h"

#include <iostream>

int main() {
    std::string data_path = "/home/vadim/Projects/mlp/data/";
    auto data_set = MNISTLoader::LoadData(data_path);

    // for (const auto& [input, output] : data_set.test_data) {
    //     std::cout << "Input\n" << input << std::endl;
    //     std::cout << "Output\n" << output << std::endl;
    // }
}