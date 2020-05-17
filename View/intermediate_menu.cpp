#include "intermediate_menu.h"

IntermediateMenu::IntermediateMenu(AbstractGameController* game_controller,
                                   MenuType menu_type,
                                   QWidget* parent)
    : Menu(game_controller, parent), menu_type_(menu_type) {

  std::shared_ptr<ImageSet> image_set = nullptr;
  switch (menu_type_) {
    case MenuType::kFail: {
      background_ = QImage(":/images/menu/big_background_pink.png");
      main_part_ = QImage(":/images/menu/fail_menu.png");
      image_set = std::make_shared<ImageSet>("malinovij");

      menu_animation_ = std::make_shared<Movie>("cloud", this);
      menu_animation_->SetSpeed(30);
      menu_animation_->Play();
      break;
    }
    case MenuType::kPause: {
      background_ = QImage(":/images/menu/big_background.png");
      main_part_ = QImage(":/images/menu/pause_menu.png");
      image_set = std::make_shared<ImageSet>("blue");
      menu_animation_ = std::make_shared<Movie>("burdoc"
                                                "k", this);
      menu_animation_->SetSpeed(100);
      menu_animation_->Play();
      break;
    }
    case MenuType::kVictory: {
      background_ = QImage(":/images/menu/big_background_yellow.png");
      main_part_ = QImage(":/images/menu/victory_menu.png");
      image_set = std::make_shared<ImageSet>("orange");

      menu_animation_ = std::make_shared<Movie>("sunflower", this);
      menu_animation_->SetSpeed(160);
      menu_animation_->Play();

      int mushrooms_count = game_controller_->GetLastLevelMushrooms();
      for (int i = 0; i < mushrooms_count; i++) {
        mushrooms_.emplace_back(std::make_shared<Movie>("mushroom", this));
        mushrooms_.back()->SetSpeed(300 - i * 75);
        mushrooms_.back()->Play();
      }
      break;
    }
    case MenuType::kControls: {
      background_ = QImage(":/images/menu/big_background.png");
      main_part_ = QImage(":/images/menu/controls.png");
      image_set = std::make_shared<ImageSet>("back_arrow");

      menu_animation_ = std::make_shared<Movie>("chestnut", this);
      menu_animation_->SetSpeed(100);
      menu_animation_->Play();
      break;
    }
  }
  scaled_background_ = background_;
  scaled_main_part_ = main_part_;

  if (menu_type_ == MenuType::kControls) {
    back_arrow_ = new Button(image_set, this);

    connect(back_arrow_, &QPushButton::clicked, this, [&]() {
      game_controller_->OpenSettingsMenu();
    });
    return;
  }

  restart_button_ = new Button(image_set, this, "Restart");
  choose_level_button_ = new Button(image_set, this, "Choose Level");
  main_menu_button_ = new Button(image_set, this, "Main Menu");

  connect(main_menu_button_, &QPushButton::clicked, this, [&]() {
    game_controller_->OpenMainMenu();
  });

  connect(choose_level_button_, &QPushButton::clicked, this, [&]() {
    game_controller_->OpenChooseLevelMenu();
  });

  connect(restart_button_, &QPushButton::clicked, this, [&]() {
    game_controller_->RestartGame();
  });

  connect(main_menu_button_, &QPushButton::pressed, this, [&]() {
    audio_manager_->PlayAudio(AudioName::kButtonClick);
  });
  connect(choose_level_button_, &QPushButton::pressed, this, [&]() {
    audio_manager_->PlayAudio(AudioName::kButtonClick);
  });
  connect(restart_button_, &QPushButton::pressed, this, [&]() {
    audio_manager_->PlayAudio(AudioName::kButtonClick);
  });

  if (menu_type_ == MenuType::kPause) {
    resume_button_ = new Button(image_set, this, "Resume");
    connect(resume_button_, &QPushButton::clicked, this, [&]() {
      game_controller_->ResumeGame();
    });

    connect(resume_button_, &QPushButton::pressed, this, [&]() {
      audio_manager_->PlayAudio(AudioName::kButtonClick);
    });
  } else if (menu_type == MenuType::kVictory) {
    resume_button_ = new Button(image_set, this, "Next Level");
    connect(resume_button_, &QPushButton::clicked, this, [&]() {
      game_controller_->StartNextLevel();
    });

    connect(resume_button_, &QPushButton::pressed, this, [&]() {
      audio_manager_->PlayAudio(AudioName::kButtonClick);
    });
  }

  if (resume_button_ != nullptr) {
    main_button_group_.push_back(resume_button_);
  }
  main_button_group_.push_back(restart_button_);
  main_button_group_.push_back(choose_level_button_);
  main_button_group_.push_back(main_menu_button_);
}

void IntermediateMenu::resizeEvent(QResizeEvent* event) {
  Menu::resizeEvent(event);

  switch (menu_type_) {
    case MenuType::kFail: {
      restart_button_->SetRectangle(PositionRectangle(1, 1, 3, 2));
      choose_level_button_->SetRectangle(PositionRectangle(12, 1, 3, 2));
      main_menu_button_->SetRectangle(PositionRectangle(12, 4, 3, 2));

      menu_animation_->setGeometry(PositionRectangle(6, 1, 4, 4));
      break;
    }
    case MenuType::kPause: {
      resume_button_->SetRectangle(PositionRectangle(7, 1, 3, 2));
      restart_button_->SetRectangle(PositionRectangle(11, 1, 3, 2));
      choose_level_button_->SetRectangle(PositionRectangle(7, 4, 3, 2));
      main_menu_button_->SetRectangle(PositionRectangle(11, 4, 3, 2));
      menu_animation_->setGeometry(PositionRectangle(1, 1, 4, 5));
      break;
    }
    case MenuType::kVictory: {
      resume_button_->SetRectangle(PositionRectangle(2, 2, 3, 2));
      restart_button_->SetRectangle(PositionRectangle(6, 2, 3, 2));
      choose_level_button_->SetRectangle(PositionRectangle(2, 5, 3, 2));
      main_menu_button_->SetRectangle(PositionRectangle(6, 5, 3, 2));

      menu_animation_->setGeometry(PositionRectangle(11, 2, 4, 5));
      int mushrooms_count = mushrooms_.size();
      for (int i = 0; i < mushrooms_count; i++) {
        mushrooms_[i]->setGeometry(PositionRectangle(10 + i, 6, 1, 1));
      }
      break;
    }
    case MenuType::kControls: {
      back_arrow_->SetRectangle(PositionRectangle(1, 1, 1, 1));

      menu_animation_->setGeometry(PositionRectangle(8, 5, 6, 2));
      break;
    }
  }

  MakeEqualFontSize(main_button_group_);
}
