#include "game_controller.h"

#include "game_object.h"
#include "view.h"

GameController::GameController() {
  view_ = new View(this);
  view_->resize(800, 600);
  view_->show();
  game_object_ = new GameObject();
}

void GameController::Update() {
  game_object_->Update();
  view_->repaint();
}

GameObject* GameController::GetGameObject() { return game_object_; }

GameController::~GameController() {
  delete view_;
  delete game_object_;
}
