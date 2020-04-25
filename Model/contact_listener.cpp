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
  EntityType type_a = *reinterpret_cast<EntityType*>(fixture_a->GetUserData());
  Entity* entity_a = GetEntityFromFixture(fixture_a, type_a);

  EntityType type_b = *reinterpret_cast<EntityType*>(fixture_b->GetUserData());
  Entity* entity_b = GetEntityFromFixture(fixture_b, type_b);

  if (contact_begin) {
    entity_a->BeginCollision(type_a, fixture_a, type_b);
    entity_b->BeginCollision(type_b, fixture_b, type_a);
  } else {
    entity_a->EndCollision(type_a, fixture_a, type_b);
    entity_b->EndCollision(type_b, fixture_b, type_a);
  }
}

Entity* ContactListener::GetEntityFromFixture(b2Fixture* fixture,
                                              EntityType entity_type) {
  switch (entity_type) {
    case EntityType::kPatroller: {
      return reinterpret_cast<Patroller*>(fixture->GetBody()->GetUserData());
    }
    case EntityType::kPlayer: {
      return reinterpret_cast<Player*>(fixture->GetBody()->GetUserData());
    }
    case EntityType::kShooter: {
      return reinterpret_cast<Shooter*>(fixture->GetBody()->GetUserData());
    }
    default: {
      return reinterpret_cast<Entity*>(fixture->GetBody()->GetUserData());
    }
  }
}
