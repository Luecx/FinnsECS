//
// Created by Finn Eggers on 07.06.24.
//

#ifndef ECS_COMPONENT_ENTITY_LIST_H
#define ECS_COMPONENT_ENTITY_LIST_H

#include "entity.h"
#include "types.h"
#include "vector_compact.h"

#include <vector>

namespace ecs {

struct ComponentEntityList : CompactVector<ID> {
    // pointer to the ECS
    std::vector<Entity>* entities_ = nullptr;

    // hash of the component
    Hash comp_hash_ = Hash{INVALID_HASH};

    // set ecs_ and component hash via some function to allow empty constructions
    void set(std::vector<Entity>* entities, Hash component_hash) {
        this->entities_      = entities;
        this->comp_hash_     = component_hash;
    }

    // overloaded
    void moved(ID from, ID to) override {
        (*entities_)[elements[to]].components[comp_hash_]->component_entity_id = to;
    }
    void removed(ID id) override {
        (*entities_)[elements[id]].components[comp_hash_]->component_entity_id = INVALID_ID;
    }
    void added(ID id) override {
        (*entities_)[elements[id]].components[comp_hash_]->component_entity_id = id;
    }

};

}    // namespace ecs_

#endif    // ECS_COMPONENT_ENTITY_LIST_H
