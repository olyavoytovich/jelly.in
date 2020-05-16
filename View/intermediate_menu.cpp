#include "intermediate_menu.h"

IntermediateMenu::IntermediateMenu(AbstractGameController* game_controller,
                                   MenuType menu_type,
                                   QWidget* parent)
    : Menu(game_controller, parent), menu_type_(menu_type) {
  background_ = QImage(":/images/menu/big_background.png");
  scaled_background_ = background_;

  std::shared_ptr<ImageSet> image_set = nullptr;
  switch (menu_type_) {
    case MenuType::kFail: {
      main_part_ = QImage(":/images/menu/fail_menu.png");
      image_set = std::make_shared<ImageSet>("malinovij");
      break;
    }
    case MenuType::kPause: {
      main_part_ = QImage(":/images/menu/pause_menu.png");
      image_set = std::make_shared<ImageSet>("blue");
      menu_animation_ = std::make_shared<Movie>("pause_menu", this);
      menu_animation_->SetSpeed(200);
      menu_animation_->Play();
      break;
    }
    case MenuType::kVictory: {
      main_part_ = QImage(":/images/menu/victory_menu.png");
      image_set = std::make_shared<ImageSet>("orange");
      break;
    }
  }
  scaled_main_part_ = main_part_;

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
      resume_button_->SetRectangle(PositionRectangle(2, 1, 3, 2));
      restart_button_->SetRectangle(PositionRectangle(6, 1, 3, 2));
      choose_level_button_->SetRectangle(PositionRectangle(2, 4, 3, 2));
      main_menu_button_->SetRectangle(PositionRectangle(6, 4, 3, 2));
      break;
    }
  }

  MakeEqualFontSize(main_button_group_);
}
