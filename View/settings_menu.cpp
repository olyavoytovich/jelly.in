#include "settings_menu.h"

SettingsMenu::SettingsMenu(AbstractGameController* game_controller,
                           QWidget* parent)
    : Menu(game_controller, parent),
      back_arrow_(new Button(std::make_shared<ImageSet>("back_arrow"), this)),
      controls_(new Button(std::make_shared<ImageSet>("controls"), this)),
      volume_(new Button(std::make_shared<ImageSet>("volume"), this)),
      reset_(new Button(std::make_shared<ImageSet>("reset"), this)) {
  background_ = QImage(":/images/menu/big_background.png");
  main_part_ = QImage(":/images/menu/settings_menu.png");
  scaled_background_ = background_;
  scaled_main_part_ = main_part_;

  menu_animation_ = std::make_shared<Movie>("bush", this);
  menu_animation_->SetSpeed(100);
  menu_animation_->Play();

  connect(back_arrow_, &QPushButton::clicked, this, [&]() {
    game_controller_->OpenMainMenu();
  });

  connect(controls_, &QPushButton::clicked, this, [&]() {
    game_controller_->OpenSettingsControls();
  });

  connect(volume_, &QPushButton::clicked, this, [&]() {
    game_controller_->OpenSettingsVolume();
  });

  connect(reset_, &QPushButton::clicked, this, [&]() {
    game_controller_->Reset();
  });
}

void SettingsMenu::resizeEvent(QResizeEvent* event) {
  Menu::resizeEvent(event);

  back_arrow_->SetRectangle(PositionRectangle(1, 1, 1, 1));
  controls_->SetRectangle(PositionRectangle(1, 3, 5, 3));
  volume_->SetRectangle(PositionRectangle(10, 2, 5, 3));
  reset_->SetRectangle(PositionRectangle(14, 1, 1, 1));

  menu_animation_->setGeometry(PositionRectangle(2, 7, 2, 1));
}
