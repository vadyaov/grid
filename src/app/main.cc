#include "mlp_window.h"
#include <QApplication>

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  MlpWindow window;
  return app.exec();
}