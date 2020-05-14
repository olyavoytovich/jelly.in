#ifndef CONTROLLER_ABSTRACT_GAME_CONTROLLER_H_
#define CONTROLLER_ABSTRACT_GAME_CONTROLLER_H_

#include <QKeyEvent>
#include <QPainter>

// kAnyKey должен быть в enum последним
enum class Key {
  kLeft,
  kUp,
  kRight,
  kSpace,
  kAnyKey
};

class AbstractGameController {
 public:
  virtual ~AbstractGameController() = default;

  virtual void Update(int time) = 0;
  virtual void Draw(QPainter* painter) const = 0;
  virtual void PressKey(int key_code) = 0;
  virtual void ClampKey(int key_code) = 0;
  virtual void ReleaseKey(int key_code) = 0;

  virtual void OpenChooseLevelMenu() = 0;
  virtual void OpenMainMenu() = 0;
  virtual void OpenPauseMenu() = 0;
  virtual void OpenVictoryMenu() = 0;
  virtual void OpenFailMenu() = 0;
  virtual void ResumeGame() = 0;
  virtual void RestartGame() = 0;
  virtual void StartNextLevel() = 0;

  virtual QString GetPlayerAnimation() const = 0;
  virtual void SetPlayerAnimation(const QString& animation_name) = 0;

  virtual void StartLevel(int level_number) = 0;
};

#endif  // CONTROLLER_ABSTRACT_GAME_CONTROLLER_H_
