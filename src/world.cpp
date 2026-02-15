// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o


#include <viotecs/types.hpp>
#include <viotecs/world.hpp>

#include <oak/oak.hpp>

#include <algorithm>

using namespace viotecs;

SetPtr<EntityId> World::entities;
UMapPtr<TypeId, Resource> World::resources;
UMapVecPtr<TypeId, Component> World::components;

template <>
void World::query_components_rec<None>(
[[maybe_unused]] std::vector<EntityId> *entities)
{
}

void World::init()
{
  World::entities = std::make_unique<std::set<EntityId>>();
  World::resources = std::make_unique<UMap<TypeId, Resource>>();
  World::components = std::make_unique<UMapVec<TypeId, Component>>();

  OAK_INFO("ecs: world initialized");
}

void World::destroy()
{
  World::entities.reset();
  World::components.reset();
  World::resources.reset();

  OAK_INFO("ecs: world deleted");
}

std::function<void()> World::run_systems;

void World::tick()
{
  if (World::run_systems)
    World::run_systems();
}

Entity World::new_entity()
{
  if (!World::entities)
  {
    return -1;
  }

  if (World::entities->empty())
  {
    World::entities->insert(1);
    return 1;
  }

  EntityId new_entity = *(World::entities->rbegin()) + 1;
  World::entities->insert(new_entity);

  OAK_DEBUG("ecs: new entity created: {}", new_entity);

  return Entity(new_entity);
}

std::set<EntityId> *World::get_entities()
{
  if (!World::entities)
  {
    return nullptr;
  }
  return World::entities.get();
}

UMap<TypeId, Resource> *World::get_resources()
{
  if (!World::resources)
  {
    return nullptr;
  }
  return World::resources.get();
}

UMapVec<TypeId, Component> *World::get_components()
{
  if (!World::components)
  {
    return nullptr;
  }
  return World::components.get();
}

void World::remove_entity(EntityId e)
{
  if (!World::entities)
  {
    return;
  }

  World::entities->erase(e);

  for (auto iter = World::components->begin(); iter != World::components->end();
       iter++)
  {
    iter->second.erase(
      std::remove_if(iter->second.begin(), iter->second.end(),
                     [&e](const std::shared_ptr<Component> &elem)
                     { return elem->entity == e; }),
      iter->second.end());
  }

  OAK_DEBUG("ecs: entity removed: {}", e);
}

EntityId Entity::id()
{
  return this->_id;
}

void Entity::remove()
{
  World::remove_entity(this->id());
  return;
}
