#pragma once

#include <QWidget>
#include <QGraphicsView>

#include "grid_scene.h"
#include "../network/network.h"

class MlpWindow : public QWidget {
  Q_OBJECT

public:
  MlpWindow(QWidget* parent = nullptr);

private:
  GridScene* grid_scene_;
  QGraphicsView* view_;

  std::unique_ptr<Network> net_;
};
