#ifndef MODEL_CONTACT_LISTENER_H_
#define MODEL_CONTACT_LISTENER_H_

#include "box2d/box2d.h"
#include "entity.h"
#include "map.h"
#include "patroller.h"
#include "player.h"
#include "pressure_plate.h"
#include "shooter.h"

class ContactListener : public b2ContactListener {
 public:
  void BeginContact(b2Contact* contact) override;
  void EndContact(b2Contact* contact) override;

 private:
  void ProcessContact(b2Contact* contact, bool contact_begin);
  Entity* GetEntityFromFixture(b2Fixture* fixture, EntityType entity_type);
};

#endif  // MODEL_CONTACT_LISTENER_H_
