#include <QApplication>

#include "game_controller.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  std::shared_ptr<GameController>
      game_controller = std::make_shared<GameController>();
  return a.exec();
}
