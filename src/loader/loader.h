#pragma once

#include <vector>
#include <string>

#include <Eigen/Dense>

#define TRAINING_DATA_SIZE 50000
#define VALIDATE_DATA_SIZE 10000
#define TEST_DATA_SIZE 10000

using Eigen::MatrixXd;
using Eigen::VectorXd;


class MNISTLoader {
public:
    struct DataSet {
        /*
            pair.second - векторизированный формат целого числа
            [0000000001] = 9
            [0000000010] = 8
            [0000000100] = 7
            ...
            [1000000000] = 0
        */
        std::vector<std::pair<VectorXd, VectorXd>> training_data;
        std::vector<std::pair<VectorXd, VectorXd>> validation_data;
        std::vector<std::pair<VectorXd, VectorXd>> test_data;
    };

    static DataSet LoadData(const std::string& data_path);

private:
    /*
    TRAINING SET IMAGE FILE (train-images-idx3-ubyte):

    [offset] [type]          [value]          [description]
    0000     32 bit integer  0x00000803(2051) magic number
    0004     32 bit integer  60000            number of images
    0008     32 bit integer  28               number of rows
    0012     32 bit integer  28               number of columns
    0016     unsigned byte   ??               pixel
    0017     unsigned byte   ??               pixel
    ........
    xxxx     unsigned byte   ??               pixel

    Pixels are organized row-wise. Pixel values are 0 to 255.
    0 means background (white), 255 means foreground (black). 
    */
    static std::vector<VectorXd> LoadImages(const std::string& filename);


    /*
    TRAINING SET LABEL FILE (train-labels-idx1-ubyte):
    [offset] [type]          [value]          [description]
    0000     32 bit integer  0x00000801(2049) magic number (MSB first)
    0004     32 bit integer  60000            number of items
    0008     unsigned byte   ??               label
    0009     unsigned byte   ??               label
    ........
    xxxx     unsigned byte   ??               label

    The labels values are 0 to 9. 
    */
    static std::vector<VectorXd> LoadLabels(const std::string& filename);
};