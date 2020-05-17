#include "main_menu.h"

MainMenu::MainMenu(AbstractGameController* game_controller, QWidget* parent)
    : Menu(game_controller, parent),
      exit_button_(new Button(std::make_shared<ImageSet>("exit"), this)),
      settings_button_(
          new Button(std::make_shared<ImageSet>("settings"), this)),
      play_button_(new Button(std::make_shared<ImageSet>("play"), this)) {
  background_ = QImage(":/images/menu/big_background.png");
  main_part_ = QImage(":/images/menu/main_menu.png");
  scaled_background_ = background_;
  scaled_main_part_ = main_part_;

  connect(play_button_, &QPushButton::clicked, this, [&]() {
    game_controller_->OpenChooseLevelMenu();
  });

  connect(exit_button_, &QPushButton::clicked, this, [&]() {
    qApp->exit();
  });

  connect(play_button_, &QPushButton::pressed, this, [&]() {
    audio_manager_->PlayAudio(AudioName::kButtonClick);
  });
  connect(settings_button_, &QPushButton::pressed, this, [&]() {
    audio_manager_->PlayAudio(AudioName::kButtonClick);
  });
  connect(exit_button_, &QPushButton::pressed, this, [&]() {
    audio_manager_->PlayAudio(AudioName::kButtonClick);
  });
}

void MainMenu::resizeEvent(QResizeEvent* event) {
  Menu::resizeEvent(event);

  play_button_->SetRectangle(PositionRectangle(7, 4, 2, 2));
  exit_button_->SetRectangle(PositionRectangle(13, 5, 1, 1));
  settings_button_->SetRectangle(PositionRectangle(2, 4, 1, 1));
}
