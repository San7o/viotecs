// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <viotecs/types.hpp>

#include <tuple>
#include <typeindex>
#include <vector>

namespace viotecs
{

/**
 * @brief System type
 *
 * This type is used to identify systems in the game world. Systems are
 * functions that process game objects and are called at any tick by the World.
 *
 * Systems can query an entity via It's components specified in the template
 * arguments. The system will be called each tick with a list of entities
 * that have all the components specified.
 *
 * Example of creating a system:
 *
 * ```
 * struct system_a : system<component_a, component_b> {
 *   void run (std::vector<entity> e) const override { std::cout << "A\n"; }
 * };
 * ```
 *
 * If no components are needed, use system<none>.
 */
template <typename... T> class system
{
public:
  using dependencies = std::tuple<T...>;
  virtual void run([[maybe_unused]]std::vector<types::entity_id> e) const {};
};

/**
 * @brief Registered Systems Type
 *
 * This class is used as a type to register systems in the World.  Use
 * the REGISTER_SYSTEMS(...) macro to register systems, this will
 * create an instance of Registered Systems with the systems
 * specified. This is used to call the systems in the World.
 *
 * Example:
 * ```
 * REGISTERED_SYSTEMS(system_a, system_b);
 * ```
 */
template <typename... T> class registered_systems
{
public:
  using systems = std::tuple<T...>;
};

/**
 * Use this macro anywhere (outside a function) to register systems.
 * Use it only once.
 */
#define REGISTER_SYSTEMS(...)                                                  \
  void viotecs::world::run_systems()                                           \
  {                                                                            \
    typedef registered_systems<__VA_ARGS__>::systems registered_systems;       \
    const registered_systems systems;                                          \
    viotecs::world::for_each(systems);                                         \
  }

} // namespace viotecs
