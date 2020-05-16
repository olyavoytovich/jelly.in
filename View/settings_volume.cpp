#include "settings_volume.h"

SettingsVolume::SettingsVolume(AbstractGameController* game_controller,
                               QWidget* parent)
    : Menu(game_controller, parent),
      back_arrow_(new Button(std::make_shared<ImageSet>("back_arrow"), this)) {
  background_ = QImage(":/images/menu/big_background.png");
  main_part_ = QImage(":/images/menu/volume_menu.png");
  scaled_background_ = background_;
  scaled_main_part_ = main_part_;

  menu_animation_ = std::make_shared<Movie>("tree", this);
  menu_animation_->SetSpeed(100);
  menu_animation_->Play();

  auto right_arrows = std::make_shared<ImageSet>("right_arrow");
  auto left_arrows = std::make_shared<ImageSet>("left_arrow");
  auto square = std::make_shared<ImageSet>("square");

  for (int i = 0; i < 3; i++) {
    right_arrows_.emplace_back(new Button(right_arrows, this));
    left_arrows_.emplace_back(new Button(left_arrows, this));
    squares_.emplace_back(new Button(square, this, "22"));
  }

  connect(back_arrow_, &QPushButton::clicked, this, [&]() {
    game_controller_->OpenSettingsMenu();
  });

  for (int i = 0; i < 3; i++) {
    connect(right_arrows_[i], &QPushButton::clicked, this, [&, i]() {
      squares_[i]->SetText("12");
    });

    connect(left_arrows_[i], &QPushButton::clicked, this, [&, i]() {
      squares_[i]->SetText("2");
    });
  }
}

void SettingsVolume::resizeEvent(QResizeEvent* event) {
  Menu::resizeEvent(event);

  int k = 2;
  for (int i = 0; i < 3; i++) {
    left_arrows_[i]->SetRectangle(PositionRectangle(6, k, 1, 1));
    right_arrows_[i]->SetRectangle(PositionRectangle(8, k, 1, 1));
    squares_[i]->SetRectangle(PositionRectangle(7, k, 1, 1));
    squares_[i]->SetLabelColor(QColor(45, 49, 56));
    k += 2;
  }

  back_arrow_->SetRectangle(PositionRectangle(1, 1, 1, 1));
  menu_animation_->setGeometry(PositionRectangle(3, 3, 1, 3));
  MakeEqualFontSize(squares_);
}
