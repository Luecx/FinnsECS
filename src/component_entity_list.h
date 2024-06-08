//
// Created by Finn Eggers on 07.06.24.
//

#ifndef ECS_COMPONENT_ENTITY_LIST_H
#define ECS_COMPONENT_ENTITY_LIST_H

#include "entity.h"
#include "types.h"
#include "self_arranging_list.h"

#include <vector>

namespace ecs {

namespace {
    struct Empty : public ComponentBaseOf<Empty> {};
}

struct ComponentEntityList : SelfArrangingList<ID> {
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
