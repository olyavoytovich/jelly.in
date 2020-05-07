#include "fail_menu.h"

FailMenu::FailMenu(AbstractGameController* game_controller, QWidget* parent)
    : Menu(game_controller, parent) {
  background_ = QImage(":/images/menu/big_background_pink.png");
  main_part_ = QImage(":/images/menu/fail_menu.png");
  scaled_background_ = background_;
  scaled_main_part_ = main_part_;

  auto image_set = std::make_shared<ImageSet>("malinovij");
  restart_button_ = new Button(image_set, this);
  choose_level_button_ = new Button(image_set, this, "Choose Level");
  main_menu_button_ = new Button(image_set, this, "Main Menu");

  connect(main_menu_button_, &QPushButton::clicked, this, [&]() {
    game_controller_->OpenMainMenu();
  });

  connect(choose_level_button_, &QPushButton::clicked, this, [&]() {
    game_controller_->OpenChooseLevelMenu();
  });
}

void FailMenu::resizeEvent(QResizeEvent* event) {
  Menu::resizeEvent(event);
  restart_button_->SetRectangle(PositionRectangle(1, 1, 3, 2));
  choose_level_button_->SetRectangle(PositionRectangle(12, 1, 3, 2));
  main_menu_button_->SetRectangle(PositionRectangle(12, 4, 3, 2));

  int font_size = height();
  if (choose_level_button_->GetFontSize() > 0) {
    font_size = std::min(font_size, choose_level_button_->GetFontSize());
  }
  if (main_menu_button_->GetFontSize() > 0) {
    font_size = std::min(font_size, main_menu_button_->GetFontSize());
  }

  choose_level_button_->SetFontSize(font_size);
  main_menu_button_->SetFontSize(font_size);
}
