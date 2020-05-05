#include "victory_menu.h"

VictoryMenu::VictoryMenu(AbstractGameController* game_controller,
                         QWidget* parent)
    : Menu(game_controller, parent) {
  background_ = QImage(":/images/menu/big_background_yellow.png");
  main_part_ = QImage(":/images/menu/victory_menu.png");
  scaled_background_ = background_;
  scaled_main_part_ = main_part_;

  auto image_set = std::make_shared<ImageSet>("orange");
  next_level_button_ = new Button(image_set, this);
  restart_button_ = new Button(image_set, this);
  choose_level_button_ = new Button(image_set, this);
  choose_level_button_->SetText("Choose Level");
  main_menu_button_ = new Button(image_set, this);
  main_menu_button_->SetText("Main Menu");

  connect(main_menu_button_, &QPushButton::clicked, this, [&]() {
    game_controller_->OpenMainMenu();
  });

  connect(choose_level_button_, &QPushButton::clicked, this, [&]() {
    game_controller_->OpenChooseLevelMenu();
  });
}

void VictoryMenu::resizeEvent(QResizeEvent* event) {
  Menu::resizeEvent(event);
  next_level_button_->setGeometry(PositionRectangle(2, 1, 3, 2));
  restart_button_->setGeometry(PositionRectangle(6, 1, 3, 2));
  choose_level_button_->setGeometry(PositionRectangle(2, 4, 3, 2));
  main_menu_button_->setGeometry(PositionRectangle(6, 4, 3, 2));
}
