// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <typeinfo>
#include <unordered_map>
#include <valfuzz/valfuzz.hpp>
#include <viotecs/viotecs.hpp>

using namespace viotecs;

struct AComponent : component
{
  int payload;
  AComponent()
  {
  }
  AComponent(int payload) : payload(payload)
  {
  }
};

TEST(entity_component, "Add a component to an entity")
{
  world::init();

  entity_t entity = world::new_entity();
  ASSERT(entity != -1);

  world::add_component<AComponent>(entity, AComponent(69));
  auto components = world::get_components();
  ASSERT(components != nullptr);
  ASSERT(components->size() == 1);
  ASSERT(components->count(type_id<AComponent>) == 1);
  ASSERT(components->at(type_id<AComponent>).size() == 1);

  auto type_components = components->at(type_id<AComponent>);
  ASSERT(type_components.size() == 1);

  auto component = static_cast<AComponent *>(type_components[0].get());
  ASSERT(component != nullptr);
  ASSERT(component->payload == 69);
  ASSERT(component->entity == entity);

  world::destroy();
}

TEST(entity_to_component, "EntityToComponent")
{
  world::init();

  entity_t entity = world::new_entity();
  ASSERT(entity != -1);

  world::add_component<AComponent>(entity, AComponent(69));
  auto component = world::entity_to_component<AComponent>(entity);
  ASSERT(component != nullptr);
  ASSERT(component->payload == 69);
  ASSERT(component->entity == entity);

  world::destroy();
}
