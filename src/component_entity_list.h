//
// Created by Finn Eggers on 07.06.24.
//

#ifndef ECS_COMPONENT_ENTITY_LIST_H
#define ECS_COMPONENT_ENTITY_LIST_H

#include "compact_vector.h"
#include "entity.h"
#include "types.h"

#include <vector>

namespace ecs {

namespace {
    struct Empty : public ComponentBaseOf<Empty> {};
}

struct ComponentEntityList : CompactVector<ID> {
    // pointer to the ECS
    ECS* ecs;

    // hash of the component
    Hash component_hash = Empty::hash();

    // set ecs and component hash via some function to allow empty constructions
    void set(ECS* ecs, Hash component_hash);

    // overloaded
    void moved(ID from, ID to) override;
    void removed(ID id) override;
    void added(ID id) override;
};

}    // namespace ecs
#endif    // ECS_COMPONENT_ENTITY_LIST_H
