#include "game_interface.h"

GameInterface::GameInterface(AbstractGameController* game_controller,
                             QWidget* parent)
    : Menu(game_controller, parent) {}

void GameInterface::SetHealth(int health) {
  if (health_.size() < health) {
    health_.resize(health);
  }

  while (current_health_ < health) {
    health_[current_health_] = std::make_shared<Movie>("heart", this);
    current_health_++;
  }
  while (current_health_ > health) {
    current_health_--;
    health_[current_health_]->SetSpeed(200);
    health_[current_health_]->Play();
    if (current_health_ == 0) {
      player_loosed_ = true;
    }
  }

  if (current_health_ == 0 && player_loosed_ && health_[0]->IsFinished()) {
    game_controller_->OpenFailMenu();
  }
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
}
