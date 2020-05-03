#include "choose_level_menu.h"

ChooseLevelMenu::ChooseLevelMenu(AbstractGameController* game_controller,
                                 QWidget* parent)
    : Menu(game_controller, parent),
      back_button_(new Button(std::make_shared<ImageSet>("back_arrow"), this)) {
  background_ = QImage(":/images/menu/big_background.png");
  main_part_ = QImage(":/images/menu/choose_level_menu.png");
  scaled_background_ = background_;
  scaled_main_part_ = main_part_;

  connect(back_button_, &QPushButton::clicked, this, [&]() {
    game_controller_->OpenMainMenu();
  });

  auto level_image_set = std::make_shared<ImageSet>("level_button");
  for (int i = 0; i < 12; i++) {
    level_buttons_.emplace_back(new Button(level_image_set, this));
  }

  for (int i = 0; i < 2; i++) {
    connect(level_buttons_[i], &QPushButton::clicked, this, [&, i]() {
      game_controller_->StartLevel(QString::number(i + 1));
    });
  }
}

void ChooseLevelMenu::resizeEvent(QResizeEvent* event) {
  Menu::resizeEvent(event);

  int i = 0;
  for (int pos_y = 1; pos_y <= 5; pos_y += 2) {
    for (int pos_x = 3; pos_x <= 12; pos_x += 3) {
      level_buttons_[i]->setGeometry(PositionRectangle(pos_x, pos_y, 2, 1));
      i++;
    }
  }

  back_button_->setGeometry(PositionRectangle(1, 1, 1, 1));
  repaint();
}
