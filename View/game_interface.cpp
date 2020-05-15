#include "game_interface.h"

GameInterface::GameInterface(AbstractGameController* game_controller,
                             QWidget* parent)
    : Menu(game_controller, parent),
      current_health_(kMaximumHealth),
      current_mushrooms_(0) {
  for (int i = 0; i < kMaximumHealth; i++) {
    health_.emplace_back(std::make_shared<Movie>("heart", this));
  }
  for (int i = 0; i < kMushroomsCount; i++) {
    mushrooms_.emplace_back(std::make_shared<Movie>("mushroom", this));
  }
}

void GameInterface::SetHealth(int health) {
  while (current_health_ > health) {
    current_health_--;
    health_[current_health_]->SetSpeed(200);
    health_[current_health_]->Play();
    if (current_health_ == 0) {
      player_lost_ = true;
    }
  }
  current_health_ = health;

  if (player_lost_ && health_[0]->IsFinished()) {
    game_controller_->OpenFailMenu();
  }
}

void GameInterface::SetMushrooms(int mushrooms) {
  while (current_mushrooms_ < mushrooms) {
    mushrooms_[current_mushrooms_]->SetSpeed(200);
    mushrooms_[current_mushrooms_]->Play();
    current_mushrooms_++;
  }
  current_mushrooms_ = mushrooms;
}

void GameInterface::resizeEvent(QResizeEvent* event) {
  Menu::resizeEvent(event);

  QRect health_position(static_cast<int>(150 * scale_),
                        static_cast<int>(150 * scale_),
                        static_cast<int>(200 * scale_),
                        static_cast<int>(200 * scale_));
  for (const auto& heart : health_) {
    heart->setGeometry(health_position);
    health_position.translate(static_cast<int>(220 * scale_), 0);
  }

  QRect mushroom_position(static_cast<int>(150 * scale_),
                          static_cast<int>(450 * scale_),
                          static_cast<int>(200 * scale_),
                          static_cast<int>(200 * scale_));
  for (const auto& mushroom : mushrooms_) {
    mushroom->setGeometry(mushroom_position);
    mushroom_position.translate(static_cast<int>(220 * scale_), 0);
  }
}
