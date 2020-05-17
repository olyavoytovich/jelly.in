#include "game_controller.h"

GameController::GameController()
    : level_mushrooms_(12, -1),
      view_(std::make_shared<View>(this)),
      menu_(std::make_shared<MainMenu>(this)) {
  view_->show();
  view_->setCentralWidget(menu_.get());
  view_->setWindowIcon(QIcon(":/images/menu/icon.png"));

  settings_ = new QSettings("View, Controller and Models", "Jelly.inl");
  player_animation_name_ = settings_->value("animation_name").toString();

  level_mushrooms_[1] = settings_->value("level_2", 0).toInt();
  for (int i = 2; i <= 11; i++) {
    level_mushrooms_[i] =
        settings_->value("level_" + QString::number(i + 1), -1).toInt();
  }
}

void GameController::Update(int time) {
  if (view_->centralWidget() != interface_.get()) {
    return;
  }
  view_->repaint();
  map_->Update(time);
  if (player_->ReachedExit()) {
    OpenVictoryMenu();
  }
  if (interface_ != nullptr) {
    last_level_mushrooms_ = map_->GetPickedMushroomsCount();
    interface_->SetMushrooms(last_level_mushrooms_);
    interface_->SetHealth(player_->GetCurrentHealth());
  }
}

void GameController::Draw(QPainter* painter) const {
  if (map_ == nullptr) {
    return;
  }
  map_->Draw(painter);
}

void GameController::PressKey(int key_code) {
  if (key_code == Qt::Key_Escape) {
    if (view_->centralWidget() == interface_.get()) {
      OpenPauseMenu();
    }
  }

  if (map_ == nullptr) {
    return;
  }
  map_->SetPressedKeyStatus(GetKeyFromCode(key_code), true);
}

void GameController::ClampKey(int key_code) {
  if (map_ == nullptr) {
    return;
  }
  map_->SetClampedKeyStatus(GetKeyFromCode(key_code), true);
}

void GameController::ReleaseKey(int key_code) {
  if (map_ == nullptr) {
    return;
  }
  Key key = GetKeyFromCode(key_code);
  map_->SetPressedKeyStatus(key, false);
  map_->SetClampedKeyStatus(key, false);
}

Key GameController::GetKeyFromCode(int key_code) {
  if (key_code == Qt::Key_Left || key_code == Qt::Key_A || key_code == 1060) {
    return Key::kLeft;
  }
  if (key_code == Qt::Key_Right || key_code == Qt::Key_D || key_code == 1042) {
    return Key::kRight;
  }
  if (key_code == Qt::Key_Up || key_code == Qt::Key_W || key_code == 1062) {
    return Key::kUp;
  }
  if (key_code == Qt::Key_Space) {
    return Key::kSpace;
  }
  return Key::kAnyKey;
}

void GameController::CloseCurrentLevel() {
  if (map_ != nullptr) {
    level_mushrooms_[level_number_] = std::max(level_mushrooms_[level_number_],
                                               map_->GetPickedMushroomsCount());
    // Если собрали не меньше 2х грибов, то открываем следующий уровень
    if (level_number_ + 1 != level_mushrooms_.size()
        && level_mushrooms_[level_number_] >= 2
        && level_mushrooms_[level_number_ + 1] == -1) {
      level_mushrooms_[level_number_ + 1] = 0;
      settings_->setValue("level_" + QString::number(level_number_ + 2), 0);
    }
  }
  settings_->setValue("level_" + QString::number(level_number_ + 1),
                      level_mushrooms_[level_number_]);
  interface_ = nullptr;
  map_ = nullptr;
  player_ = nullptr;
}

void GameController::OpenChooseLevelMenu() {
  OpenMenu(std::make_shared<ChooseLevelMenu>(this));
  CloseCurrentLevel();
}

void GameController::OpenMainMenu() {
  OpenMenu(std::make_shared<MainMenu>(this));
  CloseCurrentLevel();
}

void GameController::OpenPauseMenu() {
  OpenMenu(std::make_shared<IntermediateMenu>(this, MenuType::kPause));
}

void GameController::OpenVictoryMenu() {
  OpenMenu(std::make_shared<IntermediateMenu>(this, MenuType::kVictory));
  CloseCurrentLevel();
}

void GameController::OpenFailMenu() {
  OpenMenu(std::make_shared<IntermediateMenu>(this, MenuType::kFail));
  CloseCurrentLevel();
}

void GameController::OpenSettingsControls() {
  OpenMenu(std::make_shared<IntermediateMenu>(this, MenuType::kControls));
  CloseCurrentLevel();
}

void GameController::OpenSettingsMenu() {
  OpenMenu(std::make_shared<SettingsMenu>(this));
  CloseCurrentLevel();
}

void GameController::OpenSettingsVolume() {
  OpenMenu(std::make_shared<SettingsVolume>(this));
  CloseCurrentLevel();
}

void GameController::ResumeGame() {
  view_->takeCentralWidget();
  view_->setCentralWidget(interface_.get());
}

void GameController::RestartGame() {
  StartLevel(level_number_);
}

void GameController::StartNextLevel() {
  StartLevel(level_number_ + 1);
}

void GameController::Reset() {
  level_mushrooms_.assign(12, -1);
  level_mushrooms_[1] = 0;
  settings_->setValue("level_2", 0);
  for (int i = 2; i <= 11; i++) {
    settings_->setValue("level_" + QString::number(i + 1), -1);
  }
}

void GameController::OpenMenu(std::shared_ptr<Menu> menu) {
  menu_ = std::move(menu);
  if (menu_ != nullptr) {
    view_->takeCentralWidget();
    view_->setCentralWidget(menu_.get());
  }
}

void GameController::StartLevel(int level_number) {
  if (level_mushrooms_[level_number] == -1) {
    // Уровень заблокирован, нельзя его загружать. Уходим
    return;
  }

  map_ = MapLoader::LoadMap("level_" + QString::number(level_number + 1));
  if (map_ == nullptr) {
    return;
  }
  level_number_ = level_number;
  player_ = std::dynamic_pointer_cast<Player>(map_->GetPlayer());
  player_->SetAnimationName(player_animation_name_);
  interface_ = std::make_shared<GameInterface>(this);
  view_->takeCentralWidget();
  view_->setCentralWidget(interface_.get());
}

QString GameController::GetPlayerAnimation() const {
  return player_animation_name_;
}

void GameController::SetPlayerAnimation(const QString& animation_name) {
  player_animation_name_ = animation_name;
  settings_->setValue("animation_name", player_animation_name_);
}

int GameController::GetLastLevelMushrooms() const {
  return last_level_mushrooms_;
}

int GameController::GetLevelMushrooms(int level_number) const {
  return level_mushrooms_[level_number];
}
