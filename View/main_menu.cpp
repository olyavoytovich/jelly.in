#include "main_menu.h"

MainMenu::MainMenu(AbstractGameController* game_controller, QWidget* parent)
    : Menu(game_controller, parent) {
  background_ = QImage(":/images/menu/big_background.png");
  main_part_ = QImage(":/images/menu/main_menu_backgrond.png");
  scaled_background_ = background_;

  exit_button_ = CreateButton("exit", 416, 160, 32, 32);

  settings_button_ = CreateButton("settings", 32, 128, 32, 32);

  play_button_ = CreateButton("play", 224, 128, 64, 64);
}

void MainMenu::PressedButton(const std::shared_ptr<Button>& button) {
  if (button == exit_button_) {
    qApp->exit();
  } else if (button == play_button_) {
    game_controller_->OpenChooseLevelMenu();
  } else {
    // переход в SettingsMenu
  }
}
