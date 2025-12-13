// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <valfuzz/valfuzz.hpp>
#include <viotecs/viotecs.hpp>

using namespace viotecs;

// Used to count how many times SystemC runs
int runs = 0;

struct ComponentA : component
{
  int payload;
  ComponentA()
  {
  }
  ComponentA(int payload) : payload(payload)
  {
  }
};
struct ComponentB : component
{
  int payload;
  ComponentB()
  {
  }
  ComponentB(int payload) : payload(payload)
  {
  }
};
struct ComponentC : component
{
  int payload;
  ComponentC()
  {
  }
  ComponentC(int payload) : payload(payload)
  {
  }
};

// This system increases the payload of ComponentA and ComponentB
struct SystemA : system<ComponentA, ComponentB>
{
  void run(std::vector<types::entity_id> matched) const override
  {
    std::string test_name = "SystemA";
    ASSERT(matched.size() == 1);
    for (auto e : matched)
    {
      auto component_a = world::entity_to_component<ComponentA>(e);
      auto component_b = world::entity_to_component<ComponentB>(e);
      component_a->payload++;
      component_b->payload++;
    }
  }
};

// ComponentC is not assigned to any entity
struct SystemB : system<ComponentC>
{
  void run(std::vector<types::entity_id> matched) const override
  {
    std::string test_name = "SystemB";
    ASSERT(matched.size() == 0);
  }
};

// This system has no dependencies and should always run
struct SystemC : system<none>
{
  void run(std::vector<types::entity_id> matched) const override
  {
    std::string test_name = "SystemC";
    ASSERT(matched.size() == 0);
    runs++;
  }
};

REGISTER_SYSTEMS(SystemA, SystemB, SystemC);

TEST(systems, "Run some registered systems")
{
  world::init();

  entity e = world::new_entity();
  e.add_component<ComponentA>(69);
  world::add_component<ComponentA>(e.id(), 69);
  world::add_component<ComponentB>(e.id(), 69);

  auto component_a = e.get_component<ComponentA>();
  auto component_b = e.get_component<ComponentB>();
  ASSERT(component_a != nullptr);
  ASSERT(component_b != nullptr);
  ASSERT(component_a->payload == 69);
  ASSERT(component_b->payload == 69);

  world::tick();
  ASSERT(component_a->payload == 70);
  ASSERT(component_b->payload == 70);

  world::tick();
  ASSERT(component_a->payload == 71);
  ASSERT(component_b->payload == 71);

  world::tick();
  ASSERT(component_a->payload == 72);
  ASSERT(component_b->payload == 72);

  ASSERT(runs == 3);

  world::destroy();
}
