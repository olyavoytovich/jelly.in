#include <QApplication>

#include "game_controller.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  GameController game_controller;
  return a.exec();
}
