#ifndef ABSTRACT_GAME_CONTROLLER_H_
#define ABSTRACT_GAME_CONTROLLER_H_

#include <QPainter>
#include <QKeyEvent>

enum class Keys {
    LEFT,
    RIGHT,
    UP,
};

class AbstractGameController {
 public:
  virtual ~AbstractGameController() = default;

  virtual void Update(int time) = 0;
  virtual void Draw(QPainter* painter) const = 0;
  virtual void PressKey(QKeyEvent* event) = 0;
  virtual void ReleaseKey(QKeyEvent* event) = 0;
  virtual bool GetKey(Keys key) = 0;
};

#endif  // ABSTRACT_GAME_CONTROLLER_H_
