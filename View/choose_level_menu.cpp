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

  std::vector<std::shared_ptr<ImageSet>>
      level_images = {std::make_shared<ImageSet>("level_0"),
                      std::make_shared<ImageSet>("level_1"),
                      std::make_shared<ImageSet>("level_2"),
                      std::make_shared<ImageSet>("level_3")};
  auto locked_level_image = std::make_shared<ImageSet>("level_locked");

  for (int i = 0; i < 12; i++) {
    int level_mushrooms = game_controller->GetLevelMushrooms(i);
    if (level_mushrooms == -1) {
      level_buttons_.emplace_back(new Button(locked_level_image,
                                             this, QString::number(i + 1)));
    } else {
      level_buttons_.emplace_back(new Button(level_images[level_mushrooms],
                                             this, QString::number(i + 1)));
    }
  }

  for (int i = 0; i < 7; i++) {
    connect(level_buttons_[i], &QPushButton::clicked, this, [&, i]() {
      game_controller_->StartLevel(i);
    });
  }

  player_ = std::make_shared<Movie>("", this);
  connect(player_.get(), &QPushButton::clicked, this, [&]() {
    SetNextAnimation();
  });

  if (!game_controller_->GetPlayerAnimation().isEmpty()) {
    QString animation_name = game_controller_->GetPlayerAnimation();
    while (animation_name != kPlayerAnimations[player_animation_index_]) {
      player_animation_index_++;
    }
  }

  SetPlayerAnimation();
}

void ChooseLevelMenu::resizeEvent(QResizeEvent* event) {
  Menu::resizeEvent(event);

  int i = 0;
  for (int pos_y = 1; pos_y <= 5; pos_y += 2) {
    for (int pos_x = 3; pos_x <= 12; pos_x += 3) {
      level_buttons_[i]->SetRectangle(PositionRectangle(pos_x, pos_y, 2, 1));
      i++;
    }
  }
  MakeEqualFontSize(level_buttons_);

  back_button_->SetRectangle(PositionRectangle(1, 1, 1, 1));

  player_->setGeometry(PositionRectangle(11, 7, 1, 1));
}

void ChooseLevelMenu::SetPlayerAnimation() {
  QString animation_name = kPlayerAnimations[player_animation_index_];
  game_controller_->SetPlayerAnimation(animation_name);
  player_->SetAnimation("player_" + animation_name + "_move");
  player_->SetSpeed(300);
  player_->Play(true);
}

void ChooseLevelMenu::SetNextAnimation() {
  player_animation_index_++;
  if (player_animation_index_ == kPlayerAnimations.size()) {
    player_animation_index_ = 0;
  }
  SetPlayerAnimation();
}
