#include "main_menu.h"

MainMenu::MainMenu(const QRect& boundary_rectangle,
                   AbstractGameController* game_controller,
                   QWidget* parent)
    : Menu(boundary_rectangle, game_controller, parent) {
  background_ = QImage(":/images/backgroundBig.png");
  main_part_ = QImage(":/images/main_menu_backgrond.png");
  scaled_background_ = background_;

  QImage exit_flat(":/images/exit_first.png");
  QImage exit_pressed(":/images/exit_second.png");
  QImage exit_hovered(":/images/exit_third.png");
  QRect exit_rect(416, 160, 32, 32);
  exit_button_ =
      std::make_shared<Button>(exit_rect,
                               exit_flat,
                               exit_pressed,
                               exit_hovered);
  AddButton(exit_button_);

  QImage settings_flat(":/images/settings_first.png");
  QImage settings_pressed(":/images/settings_second.png");
  QImage settings_hovered(":/images/settings_third.png");
  QRect settings_rect(32, 128, 32, 32);
  settings_button_ = std::make_shared<Button>(settings_rect,
                                              settings_flat,
                                              settings_hovered,
                                              settings_pressed);
  AddButton(settings_button_);

  QImage play_flat(":/images/play_first.png");
  QImage play_pressed(":/images/play_second.png");
  QImage play_hovered(":/images/play_third.png");
  QRect play_rect(224, 128, 64, 64);
  play_button_ =
      std::make_shared<Button>(play_rect,
                               play_flat,
                               play_hovered,
                               play_pressed);
  AddButton(play_button_);
}

void MainMenu::PressedButton(const std::shared_ptr<Button>& button) {
  if (button == exit_button_) {
    qApp->exit();
  } else if (button == play_button_) {
    game_controller_->OpenChooseLevelMenu();
  } else {
    // start settings_menu
  }
}
