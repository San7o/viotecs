// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <oak/oak.hpp>
#include <valfuzz/valfuzz.hpp>
#include <viotecs/world.hpp>

using namespace viotecs;

BEFORE()
{
  // oak::set_level(oak::level::disabled);
  oak::set_level(oak::Level::Error);
}

AFTER()
{
}

TEST(ecs_init, "Get ECS without inizialization")
{
  auto entities = World::get_entities();
  ASSERT(entities == nullptr);

  auto resources = World::get_resources();
  ASSERT(resources == nullptr);

  auto components = World::get_components();
  ASSERT(components == nullptr);
}

TEST(ecs_init_destroy, "ECS world inizialization and destruction")
{
  World::init();

  {
    auto entities = World::get_entities();
    ASSERT(entities != nullptr);

    auto resources = World::get_resources();
    ASSERT(resources != nullptr);

    auto components = World::get_components();
    ASSERT(components != nullptr);
  }

  World::destroy();

  {
    auto entities = World::get_entities();
    ASSERT(entities == nullptr);

    auto resources = World::get_resources();
    ASSERT(resources == nullptr);

    auto components = World::get_components();
    ASSERT(components == nullptr);
  }

  World::destroy();
}
