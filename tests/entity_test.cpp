// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <valfuzz/valfuzz.hpp>
#include <viotecs/viotecs.hpp>

using namespace viotecs;

TEST(entity_test, "Create and remove and entity")
{
  World::init();

  Entity e = World::new_entity();
  ASSERT(e.id() != -1);

  auto entities = World::get_entities();
  ASSERT(entities != nullptr);
  ASSERT(entities->size() == 1);

  World::remove_entity(e.id());
  entities = World::get_entities();
  ASSERT(entities != nullptr);
  ASSERT(entities->size() == 0);

  World::destroy();
}

TEST(entities, "Create a bunch of new entities")
{
  World::init();

  EntityId e = World::new_entity().id();
  ASSERT(e != -1);
  e = World::new_entity().id();
  ASSERT(e != -1);
  e = World::new_entity().id();
  ASSERT(e != -1);
  e = World::new_entity().id();
  ASSERT(e != -1);
  e = World::new_entity().id();
  ASSERT(e != -1);
  e = World::new_entity().id();
  ASSERT(e != -1);
  e = World::new_entity().id();
  ASSERT(e != -1);
  e = World::new_entity().id();
  ASSERT(e != -1);
  e = World::new_entity().id();
  ASSERT(e != -1);

  auto entities = World::get_entities();
  ASSERT(entities != nullptr);
  ASSERT(entities->size() == 9);

  World::destroy();
}
