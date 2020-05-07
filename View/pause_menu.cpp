#include "pause_menu.h"

PauseMenu::PauseMenu(AbstractGameController* game_controller, QWidget* parent)
    : Menu(game_controller, parent) {
  background_ = QImage(":/images/menu/big_background.png");
  main_part_ = QImage(":/images/menu/pause_menu.png");
  scaled_background_ = background_;
  scaled_main_part_ = main_part_;

  auto image_set = std::make_shared<ImageSet>("blue");
  resume_button_ = new Button(image_set, this, "Resume");
  restart_button_ = new Button(image_set, this, "Restart");
  choose_level_button_ = new Button(image_set, this, "Choose Level");
  main_menu_button_ = new Button(image_set, this, "Main Menu");

  burdock_ = std::make_shared<Movie>("pause_menu", this);
  burdock_->SetSpeed(200);
  burdock_->Play();

  connect(main_menu_button_, &QPushButton::clicked, this, [&]() {
    game_controller_->OpenMainMenu();
  });

  connect(choose_level_button_, &QPushButton::clicked, this, [&]() {
    game_controller_->OpenChooseLevelMenu();
  });

  connect(resume_button_, &QPushButton::clicked, this, [&]() {
    game_controller_->ResumeGame();
  });

  connect(restart_button_, &QPushButton::clicked, this, [&]() {
    game_controller_->RestartGame();
  });

  main_button_group_.push_back(resume_button_);
  main_button_group_.push_back(restart_button_);
  main_button_group_.push_back(choose_level_button_);
  main_button_group_.push_back(main_menu_button_);
}

void PauseMenu::resizeEvent(QResizeEvent* event) {
  Menu::resizeEvent(event);

  resume_button_->SetRectangle(PositionRectangle(7, 1, 3, 2));
  restart_button_->SetRectangle(PositionRectangle(11, 1, 3, 2));
  choose_level_button_->SetRectangle(PositionRectangle(7, 4, 3, 2));
  main_menu_button_->SetRectangle(PositionRectangle(11, 4, 3, 2));

  burdock_->setGeometry(PositionRectangle(1, 1, 4, 5));

  MakeEqualFontSize(main_button_group_);
}
