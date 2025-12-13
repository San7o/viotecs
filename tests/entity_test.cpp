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

  entity_id e = world::new_entity();
  ASSERT(e != -1);

  auto entities = world::get_entities();
  ASSERT(entities != nullptr);
  ASSERT(entities->size() == 1);

  world::remove_entity(e);
  entities = world::get_entities();
  ASSERT(entities != nullptr);
  ASSERT(entities->size() == 0);

  world::destroy();
}

TEST(entities, "Create a bunch of new entities")
{
  world::init();

  entity_id e = world::new_entity();
  ASSERT(e != -1);
  e = world::new_entity();
  ASSERT(e != -1);
  e = world::new_entity();
  ASSERT(e != -1);
  e = world::new_entity();
  ASSERT(e != -1);
  e = world::new_entity();
  ASSERT(e != -1);
  e = world::new_entity();
  ASSERT(e != -1);
  e = world::new_entity();
  ASSERT(e != -1);
  e = world::new_entity();
  ASSERT(e != -1);
  e = world::new_entity();
  ASSERT(e != -1);

  auto entities = world::get_entities();
  ASSERT(entities != nullptr);
  ASSERT(entities->size() == 9);

  world::destroy();
}
