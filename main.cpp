#include <QApplication>

#include "Controller/game_controller.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName("View, Controller and Models");
  QCoreApplication::setApplicationName("Jelly.in");
  auto game_controller = std::make_shared<GameController>();
  return a.exec();
}
