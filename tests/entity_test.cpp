// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <valfuzz/valfuzz.hpp>
#include <viotecs/viotecs.hpp>

using namespace viotecs;

TEST(entity, "Create and remove and entity")
{
  world::init();

  entity_t entity = world::new_entity();
  ASSERT(entity != -1);

  auto entities = world::get_entities();
  ASSERT(entities != nullptr);
  ASSERT(entities->size() == 1);

  world::remove_entity(entity);
  entities = world::get_entities();
  ASSERT(entities != nullptr);
  ASSERT(entities->size() == 0);

  world::destroy();
}

TEST(entities, "Create a bunch of new entities")
{
  world::init();

  entity_t entity = world::new_entity();
  ASSERT(entity != -1);
  entity = world::new_entity();
  ASSERT(entity != -1);
  entity = world::new_entity();
  ASSERT(entity != -1);
  entity = world::new_entity();
  ASSERT(entity != -1);
  entity = world::new_entity();
  ASSERT(entity != -1);
  entity = world::new_entity();
  ASSERT(entity != -1);
  entity = world::new_entity();
  ASSERT(entity != -1);
  entity = world::new_entity();
  ASSERT(entity != -1);
  entity = world::new_entity();
  ASSERT(entity != -1);

  auto entities = world::get_entities();
  ASSERT(entities != nullptr);
  ASSERT(entities->size() == 9);

  world::destroy();
}
