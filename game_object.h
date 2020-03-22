#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <QRect>

class QPainter;

class GameObject {
 public:
  GameObject();
  ~GameObject() = default;

  void Update();
  void Draw(QPainter*);

 private:
  QRect rect_;
  int dx_;
};

#endif  // GAME_OBJECT_H_
