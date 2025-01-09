#include "mlp_window.h"
#include <qkeysequence.h>
#include <qnamespace.h>
#include <qshortcut.h>
#include <qsizepolicy.h>

#include <QShortcut>

#include <iostream>
#include <qwidget.h>

MlpWindow::MlpWindow(QWidget* parent) : QWidget(parent),
  grid_scene_{new GridScene(this)}
{
  view_ = new QGraphicsView(grid_scene_, this);
  view_->show();

  auto shortcut = new QShortcut(QKeySequence(Qt::Key_Escape), this);
  connect(shortcut, &QShortcut::activated, this, &QWidget::close);

  setWindowTitle("grid");
}