//
// Created by Luecx on 15.06.2021.
//

#ifndef ECS_ECS_COMPONENT_H_
#define ECS_ECS_COMPONENT_H_

#include "types.h"
#include "hash.h"
#include "ecs_base.h"

#include <ostream>
#include <type_traits>
#include <memory>

namespace ecs {

/**
 * Any component that is added to an entity must inherit from this struct.
 */
struct ComponentBase {
    // empty constructor
    ComponentBase() = default;

    ECS*        ecs                 = nullptr;
    ComponentID component_id         = ComponentID {};
    ID          component_entity_id = INVALID_ID;

    // when the component is removed from the entity
    virtual void component_removed() {};

    // when the entity is activated or deactivated
    virtual void entity_activated() {};
    virtual void entity_deactivated() {};

    // when another component is added or removed
    virtual void other_component_added(Hash hash) {};
    virtual void other_component_removed(Hash hash) {};

    // get the hash of the component
    virtual Hash get_hash() const {
        return INVALID_HASH;
    };
};

template <typename T>
struct ComponentOf : public ComponentBase {
    // empty constructor
    ComponentOf() = default;

    // implement static hash function for components
    static Hash hash() {
        return get_type_hash<T>();
    }

    // override the non-static hash function to call the static one
    virtual Hash get_hash() const override {
        return hash();
    }
};

using ComponentPtr = std::unique_ptr<ComponentBase>;

} // namespace ecs_

#endif // ECS_ECS_COMPONENT_H_
