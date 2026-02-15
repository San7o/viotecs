<h1 align=center> viotecs </h1>

The Entity Component System of [Brenta
Engine](https://github.com/San7o/Brenta-Engine).  The ECS is a design
pattern that allows you to structure your code in a way that is more
modular and scalable.

# Documentation and Installation

You can read the official online documentation
[here](https://san7o.github.io/viotecs/).

# Quick Start

## Entities

Entities are objects in the game, It's just an ID:

```c++
Entity e = World::new_entity();
```

## Components

Components are pieces of data that are attached to an entity:

```c++
struct PhysicsComponent : Component {
    float mass;
    float density;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    PhysicsComponent(float mass) : mass(mass) {}
};

// Somehwere
World::add_component<PhysicsComponent>(entity, 10.0f);

// or

e.add_component<PhysicsComponent>(10.0f);
```

## Systems

Systems are functions that operate on entities with specific
components. They are called at each game tick by the `world`:

```c++
struct FpsSystem : System<None> {
    void run(std::vector<EntityId> _) const override {
        text::render_text("FPS: " + std::to_string(time::get_fps()), 25.0f, 25.0f,
                         0.35f, glm::vec3(1.0f, 0.9f, 0.0f));
    }
};
```

To register you systems:

```c++
World::register_systems<FpsSystem>();
```

## Resources

Resources are like global data, accessible by any system:

```c++
struct MyResource : Resource {
    int my_data;
    bool is_ok;
    MyResource() {}
}
```

# Testing

The library uses the engine's testing framework
[valFuzz](https://github.com/San7o/valFuzz). To run tests, follow the
guide in [tests/README.md](./tests/README.md).

# License

The library is licensed under [MIT](./LICENSE) license.
