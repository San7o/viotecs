// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <typeinfo>
#include <unordered_map>
#include <valfuzz/valfuzz.hpp>
#include <viotecs/viotecs.hpp>

using namespace viotecs;

struct TestResource : resource
{
  int payload;
  TestResource(int payload) : payload(payload)
  {
  }
};

TEST(res, "Add, get and remove a resource from the world")
{
  world::init();

  // add the resource
  TestResource resource = TestResource(69);
  world::add_resource<TestResource>(resource);

  // get the resources
  auto resources = world::get_resources();
  ASSERT(resources != nullptr);
  ASSERT(resources->size() == 1);
  ASSERT(resources->count(type_id<TestResource>) == 1);

  // get the resource
  auto my_resource = world::get_resource<TestResource>();
  ASSERT(my_resource != nullptr);
  ASSERT(my_resource->payload == 69);

  // remove the resource
  world::remove_resource<TestResource>();
  ASSERT(resources->size() == 0);
  ASSERT(resources->count(type_id<TestResource>) == 0);

  // get the deleted resource
  my_resource = world::get_resource<TestResource>();
  ASSERT(my_resource == nullptr);

  world::destroy();
}

TEST(no_res, "Get an unexisting resource")
{
  world::init();

  // get the resource
  auto my_resource = world::get_resource<TestResource>();
  ASSERT(my_resource == nullptr);

  world::destroy();
}
