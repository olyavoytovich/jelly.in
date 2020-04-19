#ifndef MODEL_GAME_OBJECT_H_
#define MODEL_GAME_OBJECT_H_

#include <QPainter>
#include <QPolygon>
#include <utility>

#include "box2d/box2d.h"

class GameObject {
 public:
  GameObject() = default;
  explicit GameObject(QPolygon polygon, int dx = 0);
  virtual ~GameObject() = default;

  virtual void Update(int time);
  virtual void Draw(QPainter* painter) const;

  virtual void DeleteMe();
  virtual bool IsDeleted() const;

  virtual b2Body* GetB2Body() const;

 private:
  QPolygon polygon_;
  int dx_;
  bool is_object_deleted_ = false;
};

#endif  // MODEL_GAME_OBJECT_H_
