#include "contact_listener.h"

void ContactListener::BeginContact(b2Contact* contact) {
  ProcessContact(contact, true);
}

void ContactListener::EndContact(b2Contact* contact) {
  ProcessContact(contact, false);
}

void ContactListener::ProcessContact(b2Contact* contact, bool contact_begin) {
  b2Fixture* fixture_a = contact->GetFixtureA();
  b2Fixture* fixture_b = contact->GetFixtureB();
  if (fixture_a->GetUserData() == nullptr
      || fixture_b->GetUserData() == nullptr) {
    return;
  }
  EntityType type_a = *static_cast<EntityType*>(fixture_a->GetUserData());
  Entity* entity_a = GetEntityFromFixture(fixture_a, type_a);

  EntityType type_b = *static_cast<EntityType*>(fixture_b->GetUserData());
  Entity* entity_b = GetEntityFromFixture(fixture_b, type_b);

  if (contact_begin) {
    entity_a->BeginCollision(fixture_a, type_a, type_b);
    entity_b->BeginCollision(fixture_b, type_b, type_a);
  } else {
    entity_a->EndCollision(fixture_a);
    entity_b->EndCollision(fixture_b);
  }
}

Entity* ContactListener::GetEntityFromFixture(b2Fixture* fixture,
                                              EntityType entity_type) {
  if (entity_type == EntityType::kChestnut) {
    return static_cast<Patroller*>(fixture->GetBody()->GetUserData());
  } else if (entity_type == EntityType::kPlayer) {
    return static_cast<Player*>(fixture->GetBody()->GetUserData());
  } else if (entity_type == EntityType::kBurdock
      || entity_type == EntityType::kCloud
      || entity_type == EntityType::kSunflower) {
    return static_cast<Shooter*>(fixture->GetBody()->GetUserData());
  } else {
    return static_cast<Entity*>(fixture->GetBody()->GetUserData());
  }
}
