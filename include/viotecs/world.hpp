// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <algorithm>
#include <memory>
#include <oak/oak.hpp>
#include <set>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include <viotecs/component.hpp>
#include <viotecs/ecs_types.hpp>
#include <viotecs/entity.hpp>
#include <viotecs/resource.hpp>
#include <viotecs/system.hpp>

namespace viotecs
{

using type_id_t = const void *;

/**
 * Unique identifier of a type
 * Does not require RTTI
 */
template <typename T> inline constexpr type_id_t type_id = &type_id<T>;

using namespace types;

/**
 * @brief World class
 *
 * This class is a singleton that contains all the entities, components and
 * systems in the game world, provides acces to them through queries and is
 * responsible for updating the game world.
 */
class world
{
public:
  world() = delete;

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
  static std::set<entity_t> *get_entities();
  /**
   * @brief Get the resources container
   * @return The resources container, may be nullptr if not found
   */
  static UMap<type_id_t, resource> *get_resources();
  /**
   * @brief Get the components container
   * @return The components container, may be nullptr if not found
   */
  static UMapVec<type_id_t, component> *get_components();

  /**
   * @brief Create a new entity
   * @return The new entity
   */
  static entity_t new_entity();

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
   * auto resource = world::get_resource<shader>();
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
   * @param new_component The component to add
   *
   * Example:
   * ```
   * world::add_component<position>(entity, {0, 0, 0});
   * ```
   */
  template <typename C>
  static void add_component(entity_t entity, C new_component)
  {
    if (!components)
    {
      return;
    }

    auto component = std::make_shared<C>(new_component);

    component->entity = entity;

    if (!components->count(type_id<C>))
    {
      components->insert({type_id<C>, {component}});
    }
    else
    {
      components->at(type_id<C>).push_back(component);
    }

    OAK_INFO("Added component with id: {}", type_id<C>);
  }

  /**
   * @brief Add a resource to the world
   *
   * This method adds a resource to the world.
   *
   * Example:
   * ```
   * world::add_resource<shader_res>({shader});
   * ```
   */
  template <typename R> static void add_resource(R resource)
  {
    if (!resources)
    {
      return;
    }

    resources->insert({type_id<R>, std::make_shared<R>(resource)});
    OAK_INFO("Added Resource with type_id: {}", type_id<R>);
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
   * world::remove_entity(entity);
   * ```
   */
  static void remove_entity(entity_t entity);

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
   * auto component = world::entity_to_component<position_comp>(entity);
   * ```
   */
  template <typename C> static C *entity_to_component(entity_t entity)
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
      if (component->entity == entity)
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
  static void run_systems();

private:
  static SetPtr<viotecs::entity_t> entities;
  static UMapPtr<type_id_t, resource> resources;
  static UMapVecPtr<type_id_t, component> components;

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
  static std::vector<viotecs::entity_t> query_components_tuple(std::tuple<T...>)
  {
    return query_components<T...>();
  }
  template <typename System> static void process(const System &system)
  {
    using dependencies = typename System::dependencies;
    std::vector<viotecs::entity_t> matches =
      query_components_tuple(dependencies{});
    system.run(matches);
  }

  template <typename C, typename... Components, typename N = none>
  static std::vector<entity_t> query_components()
  {
    if (!world::components)
    {
      return {};
    }

    std::vector<entity_t> matched;

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

    query_components_rec<Components..., none>(&matched);

    return matched;
  }

  template <typename C, typename... Components>
  static void query_components_rec(std::vector<entity_t> *entities)
  {
    if (entities->empty())
      return;
    std::vector<entity_t> matched;

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

} // namespace viotecs
