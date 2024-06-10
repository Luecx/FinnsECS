# ECS Library Documentation

## Overview

This Entity Component System (ECS) library is designed to help manage entities, components, and systems efficiently. It allows you to create entities, attach components to them, and process these components using systems. The library is flexible and supports custom components, systems, and event listeners.

## Table of Contents

1. [Getting Started](#getting-started)
2. [Creating Entities](#creating-entities)
3. [Adding Components](#adding-components)
4. [Creating Systems](#creating-systems)
5. [Event System](#event-system)
6. [Processing the ECS](#processing-the-ecs)
7. [Examples](#examples)

## Getting Started

To start using the ECS library, include the necessary headers in your project:

```cpp
#include "ecs/include.h"
```

Ensure that your project is set up to compile the ECS library source files.

## Creating Entities

### Spawning an Entity

To create a new entity, use the `spawn` method:

```cpp
ecs::ECS ecs;
ecs::EntityID entityID = ecs.spawn(true); // Spawns an active entity
```

### Destroying an Entity

To destroy an entity, use the `destroy_entity` method:

```cpp
ecs.destroy_entity(entityID);
```

To destroy all entities:

```cpp
ecs.destroy_all_entities();
```

## Adding Components

Components must inherit from `ecs::ComponentOf<ComponentType>`. Here's how you define a custom component:

```cpp
struct MyComponent : public ecs::ComponentOf<MyComponent> {
    int value = 0;
    // Optional: Override virtual methods for lifecycle events
};
```

### Assigning a Component to an Entity

To add a component to an entity:

```cpp
ecs[entityID].assign<MyComponent>(/* constructor arguments */);
```

### Accessing a Component

To get a component from an entity:

```cpp
std::shared_ptr<MyComponent> comp = ecs[entityID].get<MyComponent>();
if (comp) {
    // Use the component
}
```

### Removing a Component

To remove a component from an entity:

```cpp
ecs[entityID].remove_component<MyComponent>();
```

To remove all components from an entity:

```cpp
ecs[entityID].remove_all_components();
```

## Creating Systems

Systems process entities and their components. Systems must inherit from `ecs::System` and implement the `process` method.

### Defining a System

```cpp
struct MySystem : public ecs::System {
    void process(ecs::ECS* ecs, double delta) override {
        for (auto& entity : ecs->each<MyComponent>()) {
            // Process entities with MyComponent
            auto comp = entity.get<MyComponent>();
            comp->value += 1;
        }
    }
};
```

### Adding a System

To add a system to the ECS:

```cpp
ecs.create_system<MySystem>();
```

### Removing a System

To remove a system, store the `SystemID` returned by `create_system` and use `destroy_system`:

```cpp
ecs::SystemID systemID = ecs.create_system<MySystem>();
ecs.destroy_system(systemID);
```

## Event System

The ECS supports event-driven programming with event listeners.

### Defining an Event and Listener

```cpp
struct MyEvent {};

struct MyEventListener : public ecs::EventListener<MyEvent> {
    void receive(ecs::ECS* ecs, const MyEvent& event) override {
        // Handle the event
    }
};
```

### Adding an Event Listener

To add an event listener:

```cpp
ecs.create_listener<MyEventListener>();
```

### Removing an Event Listener

To remove an event listener, store the `EventListenerID` and use `destroy_listener`:

```cpp
ecs::EventListenerID listenerID = ecs.create_listener<MyEventListener>();
ecs.destroy_listener(listenerID);
```

### Emitting an Event

To emit an event:

```cpp
ecs.emit_event(MyEvent{});
```

## Processing the ECS

The ECS needs to be processed to update systems. Call `process` with a delta time:

```cpp
ecs.process(0.016); // Assuming a frame time of 16ms (60 FPS)
```

## Examples

### Complete Example

```cpp
#include "ecs/ecs_core.h"
#include <iostream>

// Define components
struct Position : public ecs::ComponentOf<Position> {
    double x = 0;
    double y = 0;
};

struct Velocity : public ecs::ComponentOf<Velocity> {
    double x = 0;
    double y = 0;
};

// Define a system
struct MovementSystem : public ecs::System {
    void process(ecs::ECS* ecs, double delta) override {
        for (auto& entity : ecs->each<Position, Velocity>()) {
            auto pos = entity.get<Position>();
            auto vel = entity.get<Velocity>();
            pos->x += vel->x * delta;
            pos->y += vel->y * delta;
            std::cout << "Entity " << entity.id() << " - Position: (" << pos->x << ", " << pos->y << ")\n";
        }
    }
};

int main() {
    ecs::ECS ecs;

    // Create entities
    ecs::EntityID entity1 = ecs.spawn(true);
    ecs::EntityID entity2 = ecs.spawn(true);

    // Assign components
    ecs[entity1].assign<Position>(0.0, 0.0);
    ecs[entity1].assign<Velocity>(1.0, 0.5);
    
    ecs[entity2].assign<Position>(10.0, 10.0);
    ecs[entity2].assign<Velocity>(-0.5, -1.0);

    // Add systems
    ecs.create_system<MovementSystem>();

    // Process the ECS
    for (int i = 0; i < 10; ++i) {
        ecs.process(1.0); // Simulate 1 second per frame
    }

    return 0;
}
```

This example demonstrates creating entities with `Position` and `Velocity` components, and a `MovementSystem` that updates the position based on velocity. The ECS is processed in a loop to simulate frames.

## Conclusion

This ECS library provides a flexible framework for managing entities and components, processing systems, and handling events. By following this documentation, you should be able to effectively use the ECS in your projects. For more advanced usage and examples, refer to the source code and test cases.
