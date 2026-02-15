// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <typeinfo>
#include <unordered_map>
#include <valfuzz/valfuzz.hpp>
#include <viotecs/viotecs.hpp>

using namespace viotecs;

struct AComponent : Component
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
  World::init();

  Entity e = World::new_entity();
  ASSERT(e.id() != -1);

  e.add_component<AComponent>(69);
  auto components = World::get_components();
  ASSERT(components != nullptr);
  ASSERT(components->size() == 1);
  ASSERT(components->count(type_id<AComponent>) == 1);
  ASSERT(components->at(type_id<AComponent>).size() == 1);

  auto type_components = components->at(type_id<AComponent>);
  ASSERT(type_components.size() == 1);

  auto component = static_cast<AComponent *>(type_components[0].get());
  ASSERT(component != nullptr);
  ASSERT(component->payload == 69);
  ASSERT(component->entity == e.id());

  World::destroy();
}

TEST(entity_to_component, "EntityToComponent")
{
  World::init();

  Entity e = World::new_entity();
  ASSERT(e.id() != -1);

  e.add_component<AComponent>(69);
  auto component = e.get_component<AComponent>();
  ASSERT(component != nullptr);
  ASSERT(component->payload == 69);
  ASSERT(component->entity == e.id());

  World::destroy();
}
