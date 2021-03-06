#ifndef MODEL_GAME_OBJECT_H_
#define MODEL_GAME_OBJECT_H_

#include <QPainter>
#include <QPolygon>
#include <memory>
#include <utility>

#include "animator.h"
#include "box2d/box2d.h"

enum class EntityType {
  kPlayer = 1,
  kBullet = 2,
  kChestnut = 4,
  kCloud = 8,
  kBurdock = 16,
  kSunflower = 32,
  kGround = 64,
  kPlayerPart = 128,
  kSpikes = 256,
  kExit = 512,
  kPlate = 1024,
  kMushroom = 2048,
  kDefault
};

class GameObject {
 public:
  GameObject() = default;
  virtual ~GameObject() = default;

  virtual void Update(int time);
  virtual std::shared_ptr<Animator> GetAnimator() const = 0;
  virtual QRect GetBoundingRectangle() const = 0;
  virtual EntityType GetEntityType() const = 0;

  virtual void MarkAsDeleted();
  virtual bool IsDeleted() const;

  virtual b2Body* GetB2Body() const;
  virtual QPoint GetPositionInPixels() const = 0;

 private:
  bool is_object_deleted_ = false;
};

#endif  // MODEL_GAME_OBJECT_H_
