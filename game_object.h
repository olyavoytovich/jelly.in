#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <QPainter>
#include <QRect>

class GameObject {
 public:
  GameObject();
  ~GameObject() = default;

  void Update();
  void Draw(QPainter* painter) const;

 private:
  QRect rect_;
  int dx_;
};

#endif  // GAME_OBJECT_H_
