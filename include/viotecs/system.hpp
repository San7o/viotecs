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
template <typename... T> class System
{
public:
  using Dependencies = std::tuple<T...>;
  virtual void run([[maybe_unused]]std::vector<EntityId> e) const {};
};


/**
 * @brief Registered Systems Type
 *
 * This class is used as a type to register systems in the World. Thi
 * is used by `world::register_systems<MySystem, AnotherSystem>()` to
 * save the types of the systems.
 */
template <typename... T> class RegisteredSystems
{
public:
  using Systems = std::tuple<T...>;
};
  
} // namespace viotecs
