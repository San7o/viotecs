<h1 align=center> viotecs </h1>

The Entity Component System of [Brenta Engine](https://github.com/San7o/Brenta-Engine).
The ECS is a design pattern that allows you to structure your code in
a way that is more modular and scalable.

# Documentation and Installation

You can read the official online documentation [here](https://san7o.github.io/brenta-engine-documentation/viotecs/v1.0/).

# Quick Start

## Entities
Entities are objects in the game, It's just an ID:
```c++
entity entity = world::new_entity();
```

## Components
Components are pieces of data that are attached to an entity:
```c++
struct physics_component : component {
    float mass;
    float density;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    physics_component() {}
};

// Somehwere
world::add_component<physics_component>(entity, physics_component);
```

## Systems
Systems are functions that operate on entities with specific components. They
are called at each game tick by the `world`:
```c++
struct fps_system : system<none> {
    void run(std::vector<entity_t> _) const override {
        text::render_text("FPS: " + std::to_string(time::get_fps()), 25.0f, 25.0f,
                         0.35f, glm::vec3(1.0f, 0.9f, 0.0f));
    }
};

// Somewhere
REGISTER_SYSTEMS(fps_system);
```

## Resources

Resources are like global data, accessible by any system:
```c++
struct my_resource : resource {
    int my_data;
    bool is_ok;
    my_resource() {}
}
```

# Testing
The library uses the engine's testing framework [valFuzz](https://github.com/San7o/valFuzz). To run tests, follow the
guide in [tests/README.md](./tests/README.md).

# License

The library is licensed under [MIT](https://en.wikipedia.org/wiki/MIT_License) license.
