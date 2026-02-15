// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <typeinfo>
#include <unordered_map>
#include <valfuzz/valfuzz.hpp>
#include <viotecs/viotecs.hpp>

using namespace viotecs;

struct TestResource : Resource
{
  int payload;
  TestResource(int payload) : payload(payload)
  {
  }
};

TEST(res, "Add, get and remove a resource from the world")
{
  World::init();

  // add the resource
  TestResource resource = TestResource(69);
  World::add_resource<TestResource>(resource);

  // get the resources
  auto resources = World::get_resources();
  ASSERT(resources != nullptr);
  ASSERT(resources->size() == 1);
  ASSERT(resources->count(type_id<TestResource>) == 1);

  // get the resource
  auto my_resource = World::get_resource<TestResource>();
  ASSERT(my_resource != nullptr);
  ASSERT(my_resource->payload == 69);

  // remove the resource
  World::remove_resource<TestResource>();
  ASSERT(resources->size() == 0);
  ASSERT(resources->count(type_id<TestResource>) == 0);

  // get the deleted resource
  my_resource = World::get_resource<TestResource>();
  ASSERT(my_resource == nullptr);

  World::destroy();
}

TEST(no_res, "Get an unexisting resource")
{
  World::init();

  // get the resource
  auto my_resource = World::get_resource<TestResource>();
  ASSERT(my_resource == nullptr);

  World::destroy();
}
