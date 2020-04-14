#ifndef MODEL_GAME_OBJECT_H_
#define MODEL_GAME_OBJECT_H_

#include <QPainter>
#include <QPolygon>
#include <utility>

class GameObject {
 public:
  GameObject() = default;
  explicit GameObject(QPolygon polygon, int dx = 0);
  virtual ~GameObject() = default;

  virtual void Update(int time);
  virtual void Draw(QPainter* painter) const;

 private:
  QPolygon polygon_;
  int dx_;
};

#endif  // MODEL_GAME_OBJECT_H_
