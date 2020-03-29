#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <QPainter>
#include <QPolygon>

class GameObject {
 public:
  explicit GameObject(QPolygon polygon, int dx = 0);
  ~GameObject() = default;

  void Update();
  virtual void Draw(QPainter* painter) const;

 private:
  QPolygon polygon_;
  int dx_;
};

#endif  // GAME_OBJECT_H_