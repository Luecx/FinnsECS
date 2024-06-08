//
// Created by Finn Eggers on 07.06.24.
//

#include "component_entity_list.h"
#include "ecs.h"

void ecs::ComponentEntityList::moved(ecs::ID from, ecs::ID to) {
    ecs->entities[elements[to]].components[component_hash]->component_entity_id = to;
}
void ecs::ComponentEntityList::removed(ecs::ID id) {
    ecs->entities[elements[id]].components[component_hash]->component_entity_id = INVALID_ID;
}
void ecs::ComponentEntityList::added(ecs::ID id) {
    ecs->entities[elements[id]].components[component_hash]->component_entity_id = id;
}
void ecs::ComponentEntityList::set(ecs::ECS* ecs, ecs::Hash component_hash) {
    this->ecs = ecs;
    this->component_hash = component_hash;
}
