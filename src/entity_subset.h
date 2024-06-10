//
// Created by Finn Eggers on 10.06.24.
//

#ifndef ECS_ENTITY_SUBSET_H
#define ECS_ENTITY_SUBSET_H


#include "entity_iterator.h"

namespace ecs {

template<typename... RTypes>
struct EntitySubSet {
    std::vector<ID>* ids;
    std::vector<Entity>* entries;

    EntitySubSet(std::vector<ID>* ids, std::vector<Entity>* entries)
        : ids(ids), entries(entries) {
    }

    EntityIterator<RTypes...> begin() {
        return EntityIterator<RTypes...> {ids->begin(), ids->end(), entries};
    }

    EntityIterator<RTypes...> end() {
        return EntityIterator<RTypes...> {ids->end(), ids->end(), entries};
    }
};
}


#endif    // ECS_ENTITY_SUBSET_H
