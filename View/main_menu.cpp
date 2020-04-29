#include "main_menu.h"

MainMenu::MainMenu(QRect boundary_rectangle,
                   AbstractGameController* game_controller, QWidget* parent)
    : Menu(boundary_rectangle, game_controller, parent) {
  big_background_ = QImage(":/images/backgroundBig.png");
  small_background_ = QImage(":/images/backgroundBig.png");
  main_part_ = QImage(":/images/main_menu_backgrond.png");

  QImage exit_one(":/images/exit_first.png");
  QImage exit_two(":/images/exit_second.png");
  QImage exit_three(":/images/exit_third.png");
  QRect exit_rect(416, 160, 32, 32);
  exit_button_ =
      std::make_shared<Button>(exit_rect, exit_one, exit_two, exit_three);
  AddButton(exit_button_);

  QImage settings_one(":/images/settings_first.png");
  QImage settings_two(":/images/settings_second.png");
  QImage settings_three(":/images/settings_third.png");
  QRect settings_rect(32, 128, 32, 32);
  settings_button_ = std::make_shared<Button>(settings_rect, settings_one,
                                              settings_three, settings_two);
  AddButton(settings_button_);

  QImage play_one(":/images/play_first.png");
  QImage play_two(":/images/play_second.png");
  QImage play_three(":/images/play_third.png");
  QRect play_rect(224, 128, 64, 64);
  play_button_ =
      std::make_shared<Button>(play_rect, play_one, play_three, play_two);
  AddButton(play_button_);
}

void MainMenu::PressedButton(std::shared_ptr<Button> button) {
  if (button == exit_button_) {
    qApp->exit();
  } else if (button == play_button_) {
    game_controller_->OpenChooseLevelMenu();
  } else {
    // start settings_menu
  }
}
