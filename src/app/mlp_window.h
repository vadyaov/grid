#pragma once

#include <QWidget>
#include <QGraphicsView>

#include "grid_scene.h"
#include "../network/network_mgr.h"

class QLabel;

class MlpWindow : public QWidget {
  Q_OBJECT

public:
  explicit MlpWindow(std::unique_ptr<NetworkMgr> network, QWidget* parent = nullptr);

private:
  void onGuessClicked();
  void onResetClicked();

  GridScene* grid_scene_;
  QGraphicsView* view_;

  std::unique_ptr<NetworkMgr> network_mgr_;
  QLabel* result_label;
};
