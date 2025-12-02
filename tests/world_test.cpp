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
  //oak::set_level(oak::level::disabled);

  oak::init_writer();
  oak::set_level(oak::level::error);
}

AFTER()
{
  oak::stop_writer();
}

TEST(ecs_init, "Get ECS without inizialization")
{
  auto entities = world::get_entities();
  ASSERT(entities == nullptr);

  auto resources = world::get_resources();
  ASSERT(resources == nullptr);

  auto components = world::get_components();
  ASSERT(components == nullptr);
}

TEST(ecs_init_destroy, "ECS world inizialization and destruction")
{
  world::init();

  {
    auto entities = world::get_entities();
    ASSERT(entities != nullptr);

    auto resources = world::get_resources();
    ASSERT(resources != nullptr);

    auto components = world::get_components();
    ASSERT(components != nullptr);
  }

  world::destroy();

  {
    auto entities = world::get_entities();
    ASSERT(entities == nullptr);

    auto resources = world::get_resources();
    ASSERT(resources == nullptr);

    auto components = world::get_components();
    ASSERT(components == nullptr);
  }

  world::destroy();
}
