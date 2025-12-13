// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o


#include <viotecs/types.hpp>
#include <viotecs/world.hpp>

#include <oak/oak.hpp>

#include <algorithm>

using namespace viotecs;

SetPtr<types::entity_id> world::entities;
UMapPtr<type_id_t, resource> world::resources;
UMapVecPtr<type_id_t, component> world::components;

template <>
void world::query_components_rec<none>(
[[maybe_unused]] std::vector<types::entity_id> *entities)
{
}

void world::init()
{
  using namespace types;
  world::entities = std::make_unique<std::set<types::entity_id>>();
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

entity world::new_entity()
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

  types::entity_id new_entity = *(world::entities->rbegin()) + 1;
  world::entities->insert(new_entity);

  OAK_INFO("New entity created: {}", new_entity);

  return entity(new_entity);
}

std::set<types::entity_id> *world::get_entities()
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

void world::remove_entity(types::entity_id e)
{
  if (!world::entities)
  {
    return;
  }

  world::entities->erase(e);

  for (auto iter = world::components->begin(); iter != world::components->end();
       iter++)
  {
    iter->second.erase(
      std::remove_if(iter->second.begin(), iter->second.end(),
                     [&e](const std::shared_ptr<component> &elem)
                     { return elem->entity == e; }),
      iter->second.end());
  }

  OAK_INFO("Entity removed: {}", e);
}

types::entity_id entity::id()
{
  return this->_id;
}

void entity::remove()
{
  world::remove_entity(this->id());
  return;
}
