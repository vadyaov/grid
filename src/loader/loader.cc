#include "loader.h"

#include <iostream>

namespace {
    // From Big Endian to Little Endian using shifts
    int ReadInt(std::ifstream& file) {
        unsigned char bytes[4]; // future int value
        file.read(reinterpret_cast<char*>(bytes), 4);
        return (bytes[0] << 24 | bytes[1] << 16 | bytes[2] << 8 | bytes[3]);
    }
} // namespace

MNISTLoader::DataSet MNISTLoader::LoadData(const std::string& data_path) {
    auto training_images = LoadImages(data_path + "train-images.idx3-ubyte");
    auto training_labels = LoadLabels(data_path + "train-labels.idx1-ubyte");

    auto test_images = LoadImages(data_path + "t10k-images.idx3-ubyte");
    auto test_labels = LoadLabels(data_path + "t10k-labels.idx1-ubyte");

    DataSet data;

    data.training_data.reserve(TRAINING_DATA_SIZE);
    for (size_t i = 0; i < TRAINING_DATA_SIZE; ++i) {
        data.training_data.emplace_back(training_images[i], training_labels[i]);
    }

    data.validation_data.reserve(VALIDATE_DATA_SIZE);
    for (size_t i = VALIDATE_DATA_SIZE; i < TRAINING_DATA_SIZE + VALIDATE_DATA_SIZE; ++i) {
        data.validation_data.emplace_back(training_images[i], training_labels[i]);
    }

    data.test_data.reserve(TEST_DATA_SIZE);
    for (size_t i = 0; i < TEST_DATA_SIZE; ++i) {
        data.test_data.emplace_back(test_images[i], test_labels[i]);
    }

    return data;
}


std::vector<VectorXd> MNISTLoader::LoadImages(const std::string& filename) {
    std::cout << "Loading images data..." << std::endl;
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    int magic_number = ReadInt(file);
    int number_of_images = ReadInt(file);
    int rows = ReadInt(file);
    int cols = ReadInt(file);

    int image_size = rows * cols;

    std::cout << "magic number: " << magic_number << std::endl;
    std::cout << "number of images: " << number_of_images << std::endl;
    std::cout << "rows: " << rows << std::endl;
    std::cout << "cols: " << cols << std::endl;
    std::cout << "image_size: " << image_size << std::endl;

    std::vector<VectorXd> images;
    images.reserve(number_of_images);

    std::vector<unsigned char> buffer(image_size);
    for (int i = 0; i < number_of_images; ++i) {
        file.read(reinterpret_cast<char*>(buffer.data()), image_size);

        VectorXd image(image_size);
        for (int j = 0; j < image_size; ++j) {
            image[j] = buffer[j] / 255.0;
        }
        // std::cout << "Image " << i << std::endl;
        images.push_back(std::move(image));
    }

    return images;
}

std::vector<VectorXd> MNISTLoader::LoadLabels(const std::string& filename) {
    std::cout << "Loading labels..." << std::endl;
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    int magic_number = ReadInt(file);
    int number_of_items = ReadInt(file);

    std::cout << "magic number: " << magic_number << std::endl;
    std::cout << "number of items: " << number_of_items << std::endl;

    std::vector<unsigned char> buffer(number_of_items);
    file.read(reinterpret_cast<char*>(buffer.data()), number_of_items);

    std::vector<VectorXd> labels;
    labels.reserve(number_of_items);
    for (unsigned char label : buffer) {
        VectorXd one_hot = VectorXd::Zero(10);
        // std::cout << "Label " << static_cast<int>(label) << std::endl;
        one_hot[static_cast<int>(label)] = 1.0;

        labels.push_back(std::move(one_hot));
    }

    return labels;
}