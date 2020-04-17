#ifndef ABSTRACT_GAME_CONTROLLER_H_
#define ABSTRACT_GAME_CONTROLLER_H_

#include <QPainter>
#include <QKeyEvent>

enum class Key {
    LEFT,
    UP,
    RIGHT
};

class AbstractGameController {
 public:
  virtual ~AbstractGameController() = default;

  virtual void Update(int time) = 0;
  virtual void Draw(QPainter* painter) const = 0;
  virtual void PressKey(int key_code) = 0;
  virtual void ClampKey(int key_code) = 0;
  virtual void ReleaseKey(int key_code) = 0;
  virtual bool GetPressedKeyStatus(Key key) = 0;
  virtual bool GetClampedKeyStatus(Key key) = 0;
};

#endif  // ABSTRACT_GAME_CONTROLLER_H_
