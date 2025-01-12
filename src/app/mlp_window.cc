#include "mlp_window.h"
#include "../loader/loader.h"

#include <QShortcut>

#include <iostream>

MlpWindow::MlpWindow(QWidget* parent) : QWidget(parent),
  grid_scene_{new GridScene(this)}, net_{new Network({ROWS * COLS, 30, 10})}
{

  // auto dataset = MNISTLoader::LoadData("/home/vadim/Projects/mlp/data/");
  // net_->SGD(dataset.training_data, 30, 10, 10.0, &dataset.test_data);

  view_ = new QGraphicsView(grid_scene_);
  view_->show();

  auto shortcut = new QShortcut(QKeySequence(Qt::Key_Escape), view_);
  connect(shortcut, &QShortcut::activated, this, &QWidget::close);

  setWindowTitle("grid");
}