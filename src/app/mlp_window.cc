#include "mlp_window.h"
#include "../loader/loader.h"

#include <QShortcut>

#include <iostream>

int MlpWindow::GuessNumber() const {
  auto grid_vector = grid_scene_->GetSnapshot();
  std::cout << grid_vector.size() << std::endl;
  Eigen::Map<Eigen::VectorXd> input(grid_vector.data(), grid_vector.size());
  Eigen::MatrixXd output = net_->FeedForward(input);
  if (output.cols() != 1) {
    std::cerr << "COlS != 1. BAD" << std::endl;
  }
  Eigen::Index max_row, max_col;
  std::cout << "max_idx: " << output.maxCoeff(&max_row, &max_col) << std::endl;
  std::cout << "row: " << max_row << std::endl;
  std::cout << "col: " << max_col << std::endl;

  return max_row; 
}

MlpWindow::MlpWindow(QWidget* parent) : QWidget(parent),
  grid_scene_{new GridScene(this)}, net_{new Network({ROWS * COLS, 30, 10})}
{

  auto dataset = MNISTLoader::LoadData("/home/vadim/Projects/mlp/data/");
  for (int i = 0; i < 10; ++i) {
    std::cout << dataset.test_data[i].first.reshaped(28, 28).transpose() << "\n" << std::endl;
  }
  net_->SGD(dataset.training_data, 30, 10, 10.0, &dataset.test_data);

  view_ = new QGraphicsView(grid_scene_);
  view_->show();

  auto close_shortcut = new QShortcut(QKeySequence(Qt::Key_Escape), view_);
  connect(close_shortcut, &QShortcut::activated, this, &QWidget::close);

  auto guess_shortcut = new QShortcut(QKeySequence(Qt::Key_G), view_);
  connect(guess_shortcut, &QShortcut::activated, this, &MlpWindow::GuessNumber);

  setWindowTitle("grid");
}