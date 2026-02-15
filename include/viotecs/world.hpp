// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <viotecs/component.hpp>
#include <viotecs/types.hpp>
#include <viotecs/resource.hpp>
#include <viotecs/system.hpp>

#include <oak/oak.hpp>

#include <algorithm>
#include <memory>
#include <set>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace viotecs
{

class Entity
{
public:

  Entity() = delete;
  Entity(EntityId e) : _id(e) {}
  ~Entity() = default;

  class Builder;
  
  EntityId id();
  
  template<typename C, typename... Args>
  Entity &add_component(Args&&... args);

  template <typename C>
  C *get_component();

  void remove();

private:
  EntityId _id;

};

using TypeId = const void *;

/**
 * Unique identifier of a type
 * Does not require RTTI
 */
template <typename T> inline constexpr TypeId type_id = &type_id<T>;

/**
 * @brief World class
 *
 * This class is a singleton that contains all the entities, components and
 * systems in the game world, provides acces to them through queries and is
 * responsible for updating the game world.
 */
class World
{
public:
  World() = delete;

  /**
   * @brief Initialize the world
   *
   * This method initializes the world, creating the entities, components,
   * and resources containers.
   */
  static void init();
  /**
   * @brief Delete the world
   *
   * This method deletes the world, freeing the entities, components,
   * and resources containers, freeing the memory.
   */
  static void destroy();
  /**
   * @brief Tick the world
   *
   * This method ticks the world, calling each system in the world.
   */
  static void tick();

  /**
   * @brief Get the entities container
   * @return The entities container, may be nullptr if not found
   */
  static std::set<EntityId> *get_entities();
  /**
   * @brief Get the resources container
   * @return The resources container, may be nullptr if not found
   */
  static UMap<TypeId, Resource> *get_resources();
  /**
   * @brief Get the components container
   * @return The components container, may be nullptr if not found
   */
  static UMapVec<TypeId, Component> *get_components();

  /**
   * @brief Create a new entity
   * @return The new entity
   */
  static Entity new_entity();

  /**
   * @brief Get a pointer to a resource
   *
   * This method returns a pointer to a resource of the specified type.
   * If the resource is not found, it returns nullptr.
   *
   * @tparam R The type of the resource
   * @return A pointer to the resource
   *
   * Example:
   * ```
   * auto resource = World::get_resource<Shader>();
   * ```
   */
  template <typename R> static R *get_resource()
  {
    if (!resources)
    {
      return nullptr;
    }

    if (!resources->count(type_id<R>))
    {
      return nullptr;
    }

    auto ret = resources->at(type_id<R>);
    if (ret)
      return static_cast<R *>(ret.get());
    return nullptr;
  }

  /**
   * @brief Add a component to an entity
   *
   * This method adds a component to an entity. The component is copied
   * and stored in the world.
   *
   * @tparam C The type of the component
   * @param entity The entity to add the component to
   * @param args The argument to construct the component
   *
   * Example:
   * ```
   * World::add_component<Position>(entity, 0, 0, 0);
   * ```
   */
  template <typename C, typename... Args>
  static void add_component(EntityId e, Args&&... args)
  {
    if (!components)
    {
      return;
    }

    auto component = std::make_shared<C>(C(std::forward<Args>(args)...));

    component->entity = e;

    if (!components->count(type_id<C>))
    {
      components->insert({type_id<C>, {component}});
    }
    else
    {
      components->at(type_id<C>).push_back(component);
    }

    OAK_DEBUG("ecs: added component with id: {}", type_id<C>);
  }

  /**
   * @brief Add a resource to the world
   *
   * This method adds a resource to the world.
   *
   * Example:
   * ```
   * World::add_resource<shader_res>(shader);
   * ```
   */
  template <typename R> static void add_resource(R resource)
  {
    if (!resources)
    {
      return;
    }

    resources->insert({type_id<R>, std::make_shared<R>(resource)});
    OAK_DEBUG("ecs: added Resource with type_id: {}", type_id<R>);
  }

  /**
   * @brief Remove an entity
   *
   * This method removes an entity from the world, deleting all its
   * components.
   *
   * @param entity The entity to remove
   *
   * Example:
   * ```
   * World::remove_entity(entity);
   * ```
   */
  static void remove_entity(EntityId e);

  /**
   * @brief Remove a resource
   *
   * This method removes a resource of the specified type from the world.
   *
   * @tparam R The type of the resource
   *
   * Example:
   * ```
   * World::RemoveResource<Shader>();
   * ```
   */
  template <typename R> static void remove_resource()
  {
    if (!resources)
    {
      return;
    }

    if (!resources->count(type_id<R>))
    {
      return;
    }

    resources->erase(type_id<R>);
  }

  /**
   * @brief Get the component of an entity
   *
   * This method returns a pointer to the component of the specified type
   * of the specified entity. If the component is not found, it returns
   * nullptr.
   *
   * @tparam C The type of the component
   * @param entity The entity to get the component from
   * @return A pointer to the component
   *
   * Example:
   * ```
   * auto component = World::entity_to_component<PositionComponent>(e);
   * ```
   */
  template <typename C> static C *entity_to_component(EntityId e)
  {
    if (!components)
    {
      return nullptr;
    }

    if (!components->count(type_id<C>))
    {
      return nullptr;
    }

    for (auto component : components->at(type_id<C>))
    {
      if (component->entity == e)
      {
        return static_cast<C *>(component.get());
      }
    }

    return nullptr;
  }

  /**
   * @brief Run all systems
   *
   * This method runs all the systems in the world.
   */
  static std::function<void()> run_systems;

  template<typename... S>
  static void register_systems()
  {
    run_systems = []() {
      using List = typename RegisteredSystems<S...>::Systems;
      for_each(List{});
    };
  }
  
private:
  static SetPtr<EntityId> entities;
  static UMapPtr<TypeId, Resource> resources;
  static UMapVecPtr<TypeId, Component> components;

  // Iterate over all systems and run them
  template <typename Tuple, std::size_t... Is>
  static void for_each_impl(Tuple &&tuple, std::index_sequence<Is...>)
  {
    (..., process(std::get<Is>(std::forward<Tuple>(tuple))));
  }
  template <typename Tuple> static void for_each(Tuple &&tuple)
  {
    for_each_impl(std::forward<Tuple>(tuple),
                  std::make_index_sequence<
                    std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
  }
  template <typename... T>
  static std::vector<EntityId> query_components_tuple(std::tuple<T...>)
  {
    return query_components<T...>();
  }
  template <typename System> static void process(const System &system)
  {
    using Dependencies = typename System::Dependencies;
    std::vector<EntityId> matches =
      query_components_tuple(Dependencies{});
    system.run(matches);
  }

  template <typename C, typename... Components, typename N = None>
  static std::vector<EntityId> query_components()
  {
    if (!World::components)
    {
      return {};
    }

    std::vector<EntityId> matched;

    if (components->count(type_id<C>) == 0)
    {
      return matched;
    }

    for (auto component : components->at(type_id<C>))
    {
      matched.push_back(component->entity);
    }

    if (matched.empty())
      return matched;
    if (sizeof...(Components) == 0)
      return matched;

    query_components_rec<Components..., None>(&matched);

    return matched;
  }

  template <typename C, typename... Components>
  static void query_components_rec(std::vector<EntityId> *entities)
  {
    if (entities->empty())
      return;
    std::vector<EntityId> matched;

    if (components->count(type_id<C>) == 0)
    {
      return;
    }
    for (auto component : components->at(type_id<C>))
    {
      if (std::find(entities->begin(), entities->end(),
                    (int &) component->entity)
          != entities->end())
      {
        matched.push_back(component->entity);
      }
    }
    *entities = matched;

    if (sizeof...(Components) == 0)
      return;

    (query_components_rec<Components>(entities), ...);
  }
};

template<typename C, typename... Args>
Entity &Entity::add_component(Args&&... args) {
  World::add_component<C>(this->id(), std::forward<Args>(args)...);
  return *this;
}

template <typename C>
C *Entity::get_component()
{
  return World::entity_to_component<C>(this->id());
}

  
} // namespace viotecs
