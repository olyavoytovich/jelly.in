#include "choose_level_menu.h"

ChooseLevelMenu::ChooseLevelMenu(AbstractGameController* game_controller,
                                 QWidget* parent)
    : Menu(game_controller, parent) {
  background_ = QImage(":/images/menu/big_background.png");
  main_part_ = QImage(":/images/menu/shoose_level_menu_background.png");
  scaled_background_ = background_;

  back_button_ = CreateButton("back_arrow", 32, 32, 32, 32);

  int pos_x = 0;
  int pos_y = -1;
  for (int i = 1; i <= 12; i++) {
    pos_x += 3;
    if (i % 4 == 1) {
      pos_y += 2;
      pos_x = 3;
    }
    auto button = CreateButton("level_button", pos_x * 32, pos_y * 32, 64, 32);
    buttons_.push_back(button);
  }
}

void ChooseLevelMenu::PressedButton(const std::shared_ptr<Button>& button) {
  if (button == back_button_) {
    game_controller_->CloseMainMenu();
  } else {
    // start level
  }
}
