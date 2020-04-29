#include "choose_level_menu.h"

ChooseLevelMenu::ChooseLevelMenu(QRect boundary_rectangle,
                                 AbstractGameController* game_controller,
                                 QWidget* parent)
    : Menu(boundary_rectangle, game_controller, parent) {
  big_background_ = QImage(":/images/backgroundBig.png");
  small_background_ = QImage(":/images/backgroundBig.png");
  main_part_ = QImage(":/images/shoose_level_menu_background.png");

  QImage back_arrow_one(":/images/back_arrow_first.png");
  QImage back_arrow_two(":/images/back_arrow_second.png");
  QRect back_rect(32, 32, 32, 32);
  back_button_ = std::make_shared<Button>(back_rect, back_arrow_one,
                                          back_arrow_one, back_arrow_two);
  AddButton(back_button_);

  QImage level_one(":/images/level_button_first.png");
  QImage level_two(":/images/level_button_second.png");

  QRect level_rect_1(96, 32, 64, 32);
  first_level_button_ =
      std::make_shared<Button>(level_rect_1, level_one, level_one, level_two);
  AddButton(first_level_button_);

  QRect level_rect_2(192, 32, 64, 32);
  second_level_button_ =
      std::make_shared<Button>(level_rect_2, level_one, level_one, level_two);
  AddButton(second_level_button_);

  QRect level_rect_3(288, 32, 64, 32);
  third_level_button_ =
      std::make_shared<Button>(level_rect_3, level_one, level_one, level_two);
  AddButton(third_level_button_);

  QRect level_rect_4(384, 32, 64, 32);
  fourth_level_button_ =
      std::make_shared<Button>(level_rect_4, level_one, level_one, level_two);
  AddButton(fourth_level_button_);

  //-------------------------------------------------------------------------

  QRect level_rect_5(96, 96, 64, 32);
  fifth_level_button_ =
      std::make_shared<Button>(level_rect_5, level_one, level_one, level_two);
  AddButton(fifth_level_button_);

  QRect level_rect_6(192, 96, 64, 32);
  sixth_level_button_ =
      std::make_shared<Button>(level_rect_6, level_one, level_one, level_two);
  AddButton(sixth_level_button_);

  QRect level_rect_7(288, 96, 64, 32);
  seventh_level_button_ =
      std::make_shared<Button>(level_rect_7, level_one, level_one, level_two);
  AddButton(seventh_level_button_);

  QRect level_rect_8(384, 96, 64, 32);
  eighth_level_button_ =
      std::make_shared<Button>(level_rect_8, level_one, level_one, level_two);
  AddButton(eighth_level_button_);

  //-------------------------------------------------------------------------

  QRect level_rect_9(96, 160, 64, 32);
  ninth_level_button_ =
      std::make_shared<Button>(level_rect_9, level_one, level_one, level_two);
  AddButton(ninth_level_button_);

  QRect level_rect_10(192, 160, 64, 32);
  tenth_level_button_ =
      std::make_shared<Button>(level_rect_10, level_one, level_one, level_two);
  AddButton(tenth_level_button_);

  QRect level_rect_11(288, 160, 64, 32);
  eleventh_level_button_ =
      std::make_shared<Button>(level_rect_11, level_one, level_one, level_two);
  AddButton(eleventh_level_button_);

  QRect level_rect_12(384, 160, 64, 32);
  tvelvth_level_button_ =
      std::make_shared<Button>(level_rect_12, level_one, level_one, level_two);
  AddButton(tvelvth_level_button_);
}

void ChooseLevelMenu::PressedButton(std::shared_ptr<Button> button) {
  if (button == back_button_) {
    game_controller_->CloseChooseLevelMenu();
  } else {
      // start level
  }
}
