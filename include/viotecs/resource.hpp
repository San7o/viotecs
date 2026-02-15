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
 * Resources are pieces of global data that can be accessed by any
 * System.
 *
 * Example creating a resource:
 *
 * ```
 * struct WireframeResource : Resource {
 *     bool enabled;
 *     WireframeResource(bool e) : enabled(e) {}
 * };
 * ```
 *
 * Example registering a resource:
 * ```
 * World::add_resource<WireframeResource>(false);
 * ```
 *
 */
class Resource
{
};

} // namespace viotecs
