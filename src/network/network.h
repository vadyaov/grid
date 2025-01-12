#pragma once

#include <vector>

#include <Eigen/Dense>

using Eigen::MatrixXd;
using Eigen::VectorXd;

typedef std::pair<VectorXd, VectorXd> VectorXdPair;

class Network {
public:
  Network(std::vector<int> &&layers);

  MatrixXd FeedForward(MatrixXd a) const;

  /// Стохастический градиентный спуск
  /// Принимает обучающие данные (пары вход-выход), количество эпох, размер
  /// мини-батча, скорость обучения Для каждой эпохи
  ///   - перемешивает обучающие данные случайным образом
  ///   - разбивает данные на мини-батчи заданного размера
  ///   - для каждого мини-батча вызывает updateminibutch
  ///   - опционально проверяет точность на тестовых данных
  void SGD(std::vector<VectorXdPair> &training_data, size_t epochs,
           size_t mini_batch_size, double eta,
           const std::vector<VectorXdPair> *test_data = nullptr);

  /// Создает нулевые матрицы для накопления градиентов той же размерности, что
  /// веса и смещения. Для каждой пары в мини-батче вызывает backprop для
  /// получения градиентов, накапливает градиенты. Обновляет веса и смещения по
  /// формуле параметр = параметр - (скорость обучения/размер батча)*градиент
  void UpdateMiniBatch(const std::vector<VectorXdPair> &mini_batch, double eta);

  std::pair<std::vector<MatrixXd>, std::vector<MatrixXd>>
  Backprop(const Eigen::VectorXd &x, const Eigen::VectorXd &y);

  int Evaluate(const std::vector<VectorXdPair> &test_data);

  void dump() const noexcept;

private:
  int layers_;                    // number of layers
  std::vector<int> sizes_;        // number of neurons in layers
  std::vector<MatrixXd> biases_;  // biases for layers [1, sizes)
  std::vector<MatrixXd> weights_; // weights between layers

  static MatrixXd Sigmoid(const MatrixXd &z);
  static MatrixXd SigmoidPrime(const MatrixXd &z);
  static MatrixXd CostDeriative(const MatrixXd &output_actiavtions,
                                const MatrixXd &y);
};