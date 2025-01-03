#include "mlp_window.h"
#include <qsizepolicy.h>

MlpWindow::MlpWindow(QWidget* parent) : QWidget(parent),
  grid_scene_{new GridScene(this)}
{
  view_ = new QGraphicsView(grid_scene_, this);
  view_->show();

  setWindowTitle("grid");
}
