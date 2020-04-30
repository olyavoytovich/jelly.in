#include "choose_level_menu.h"

ChooseLevelMenu::ChooseLevelMenu(const QRect& boundary_rectangle,
                                 AbstractGameController* game_controller,
                                 QWidget* parent)
    : Menu(boundary_rectangle, game_controller, parent) {
  background_ = QImage(":/images/backgroundBig.png");
  main_part_ = QImage(":/images/shoose_level_menu_background.png");
  scaled_background_ = background_;

  QImage back_arrow_flat(":/images/back_arrow_first.png");
  QImage back_arrow_hovered(":/images/back_arrow_second.png");
  QRect back_rect(32, 32, 32, 32);
  back_button_ = std::make_shared<Button>(back_rect, back_arrow_flat,
                                          back_arrow_flat, back_arrow_hovered);
  AddButton(back_button_);

  QImage level_flat(":/images/level_button_first.png");
  QImage level_hovered(":/images/level_button_second.png");

  int pos_x = 0;
  int pos_y = -1;
  for (int i = 1; i <= 12; i++) {
    pos_x += 3;
    if (i % 4 == 1) {
      pos_y += 2;
      pos_x = 3;
    }
    QRect level_rect = QRect(pos_x * 32, pos_y * 32, 64, 32);
    auto button =
        std::make_shared<Button>(level_rect,
                                 level_flat,
                                 level_flat,
                                 level_hovered);
    buttons_.push_back(button);
    AddButton(button);
  }
}

void ChooseLevelMenu::PressedButton(const std::shared_ptr<Button>& button) {
  if (button == back_button_) {
    game_controller_->CloseChooseLevelMenu();
  } else {
    // start level
  }
}
