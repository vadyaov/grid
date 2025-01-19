#include "mlp_window.h"
#include <QApplication>

#include "../network/network_mgr.h"

template<typename... Ts>
auto CreateNetworkMgr(Ts&&... params) -> std::unique_ptr<NetworkMgr> {
  return std::make_unique<NetworkMgr>(std::forward<Ts>(params)...);
}

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  auto net_mgr = CreateNetworkMgr(std::vector<int> {784, 30, 10});
  net_mgr->train({"/home/vadim/Projects/mlp/data/", 30, 10, 10.0});

  MlpWindow window(std::move(net_mgr));
  window.show();
  return app.exec();
}