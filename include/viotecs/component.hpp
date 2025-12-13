// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <viotecs/types.hpp>

namespace viotecs
{

/**
 * @brief Component class
 *
 * This class is used to create components. Components are used to store
 * data that is associated with an entity. For example, a Position
 * component could store the position of the entity.
 *
 * Example creating a component:
 *
 * ```
 * struct transform_component : component {
 *   glm::vec3 position;
 *   glm::vec3 rotation;
 *   float scale;
 *
 *   transform_component() : ...
 *   transform_component(glm::vec3 position, ...
 * };
 * ```
 *
 * You need to provide a default constructor,
 * any other constructor is optional.
 */
class component
{
public:
  types::entity_id entity;
  
  bool operator==(const component &other) const
  {
    return (entity == other.entity);
  }
};

} // namespace viotecs
