#pragma once

#include <QWidget>
#include <QGraphicsView>

#include "grid_scene.h"

class MlpWindow : public QWidget {
  Q_OBJECT

public:
  MlpWindow(QWidget* parent = nullptr);

private:
  GridScene* grid_scene_;
  QGraphicsView* view_;
};
