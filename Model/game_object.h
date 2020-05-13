#ifndef MODEL_GAME_OBJECT_H_
#define MODEL_GAME_OBJECT_H_

#include <QPainter>
#include <QPolygon>
#include <utility>

#include "box2d/box2d.h"
#include "sound_manager.h"

class GameObject {
 public:
  GameObject() = default;
  virtual ~GameObject() = default;

  virtual void Update(int time);
  virtual void Draw(QPainter* painter) const;

  virtual void MarkAsDeleted();
  virtual bool IsDeleted() const;

  virtual b2Body* GetB2Body() const;
  virtual QPoint GetPositionInPixels() const = 0;



 private:
  bool is_object_deleted_ = false;
};

#endif  // MODEL_GAME_OBJECT_H_
