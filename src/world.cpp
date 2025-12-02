// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <algorithm>
#include <oak/oak.hpp>
#include <viotecs/world.hpp>

using namespace viotecs;

SetPtr<entity_t> world::entities;
UMapPtr<type_id_t, resource> world::resources;
UMapVecPtr<type_id_t, component> world::components;

template <>
void world::query_components_rec<none>(
  [[maybe_unused]] std::vector<entity_t> *entities)
{
}

void world::init()
{
  using namespace types;
  world::entities = std::make_unique<std::set<entity_t>>();
  world::resources = std::make_unique<UMap<type_id_t, resource>>();
  world::components = std::make_unique<UMapVec<type_id_t, component>>();

  OAK_INFO("World initialized");
}

void world::destroy()
{
  world::entities.reset();
  world::components.reset();
  world::resources.reset();

  OAK_INFO("World deleted");
}

void world::tick()
{
  world::run_systems();
}

entity_t world::new_entity()
{
  if (!world::entities)
  {
    return -1;
  }

  if (world::entities->empty())
  {
    world::entities->insert(1);
    return 1;
  }

  entity_t new_entity = *(world::entities->rbegin()) + 1;
  world::entities->insert(new_entity);

  OAK_INFO("New entity created: {}", new_entity);

  return new_entity;
}

std::set<entity_t> *world::get_entities()
{
  if (!world::entities)
  {
    return nullptr;
  }
  return world::entities.get();
}

UMap<type_id_t, resource> *world::get_resources()
{
  if (!world::resources)
  {
    return nullptr;
  }
  return world::resources.get();
}

UMapVec<type_id_t, component> *world::get_components()
{
  if (!world::components)
  {
    return nullptr;
  }
  return world::components.get();
}

void world::remove_entity(entity_t entity)
{
  if (!world::entities)
  {
    return;
  }

  world::entities->erase(entity);

  for (auto iter = world::components->begin(); iter != world::components->end();
       iter++)
  {
    iter->second.erase(
      std::remove_if(iter->second.begin(), iter->second.end(),
                     [&entity](const std::shared_ptr<component> &elem)
                     { return elem->entity == entity; }),
      iter->second.end());
  }

  OAK_INFO("Entity removed: {}", entity);
}
