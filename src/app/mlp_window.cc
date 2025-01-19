#include "mlp_window.h"

#include <QShortcut>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QSizePolicy>

#include <iostream>

MlpWindow::MlpWindow(std::unique_ptr<NetworkMgr> net_mgr , QWidget* parent) : QWidget(parent),
  grid_scene_{new GridScene},
  view_{new QGraphicsView(grid_scene_, this)},
  network_mgr_{std::move(net_mgr)},
  result_label(new QLabel)
{
  QPushButton* guess_button = new QPushButton("Guess");
  QPushButton* reset_button = new QPushButton("Reset");

  result_label->setAlignment(Qt::AlignCenter);

  QGridLayout* main_layout = new QGridLayout(this);
  main_layout->addWidget(view_, 0, 0, 1, 3);

  main_layout->addWidget(guess_button, 1, 0);
  main_layout->addWidget(result_label, 1, 1);
  main_layout->addWidget(reset_button, 1, 2);

  auto close_shortcut = new QShortcut(QKeySequence(Qt::Key_Escape), view_);
  connect(close_shortcut, &QShortcut::activated, this, &QWidget::close);

  connect(guess_button, &QPushButton::clicked, this, &MlpWindow::onGuessClicked);
  connect(reset_button, &QPushButton::clicked, this, &MlpWindow::onResetClicked);

  view_->show();
  setFixedSize(sizeHint());
  setWindowTitle("grid");
}

void MlpWindow::onGuessClicked() {
  std::vector<double> snapshot = grid_scene_->GetSnapshot();
  int guessResult = network_mgr_->recognize(snapshot);
  result_label->setText(QString::number(guessResult));
}

void MlpWindow::onResetClicked() {
  grid_scene_->refreshState();
  result_label->clear();
}