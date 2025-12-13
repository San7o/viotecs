// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

namespace viotecs
{

/**
 * @brief Resource type
 *
 * This type is used to identify resources in the game world.
 * Resources are pieces of global data that can be accessed by any System.
 *
 * Example creating a resource:
 *
 * ```
 * struct wireframe_resource : resource {
 *     bool enabled;
 *     wireframe_resource(bool e) : enabled(e) {}
 * };
 * ```
 *
 * Example registering a resource:
 * ```
 * world::add_resource<wireframe_resource>(wireframe_resource(false));
 * ```
 *
 */
class resource
{
};

} // namespace viotecs
