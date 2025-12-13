// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <valfuzz/valfuzz.hpp>
#include <viotecs/viotecs.hpp>

using namespace viotecs;

TEST(entity_test, "Create and remove and entity")
{
  world::init();

  entity e = world::new_entity();
  ASSERT(e.id() != -1);

  auto entities = world::get_entities();
  ASSERT(entities != nullptr);
  ASSERT(entities->size() == 1);

  world::remove_entity(e.id());
  entities = world::get_entities();
  ASSERT(entities != nullptr);
  ASSERT(entities->size() == 0);

  world::destroy();
}

TEST(entities, "Create a bunch of new entities")
{
  world::init();

  types::entity_id e = world::new_entity().id();
  ASSERT(e != -1);
  e = world::new_entity().id();
  ASSERT(e != -1);
  e = world::new_entity().id();
  ASSERT(e != -1);
  e = world::new_entity().id();
  ASSERT(e != -1);
  e = world::new_entity().id();
  ASSERT(e != -1);
  e = world::new_entity().id();
  ASSERT(e != -1);
  e = world::new_entity().id();
  ASSERT(e != -1);
  e = world::new_entity().id();
  ASSERT(e != -1);
  e = world::new_entity().id();
  ASSERT(e != -1);

  auto entities = world::get_entities();
  ASSERT(entities != nullptr);
  ASSERT(entities->size() == 9);

  world::destroy();
}
