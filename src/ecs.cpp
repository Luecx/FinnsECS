//
// Created by Finn Eggers on 31.05.24.
//
#include "ecs.h"

ecs::ID ecs::ECS::spawn(bool active) {
    entities.push_back(Entity(this));
    entities.back().entity_id = entities.size() - 1;

    if (active) {
        entities.back().activate();
    }

    return entities.back().entity_id;
}

void ecs::ECS::destroy_entity(ecs::ID id) {
    // get the entity at the given id
    auto entity = &entities[id];

    // if active, deactivate (will notify components)
    entity->deactivate();

    // destroy the components
    entity->remove_all_components();

    // set the entity's id to INVALID_ID and the pointer to nullptr
    entity->entity_id = INVALID_ID;
}

void ecs::ECS::destroy_all_entities() {
    for (Entity& entity : entities) {
        destroy_entity(entity.entity_id);
    }
    entities.clear();
}

void ecs::ECS::destroy_all_systems() {
    for (System* sys : systems) {
        sys->destroy();
    }
    systems.clear();
}

void ecs::ECS::component_removed(ecs::Hash hash, ID id) {
    if (entities[id].active()) {
        remove_from_component_list(id, hash);
    }
}
void ecs::ECS::component_added(ecs::Hash hash, ID id) {
    if (entities[id].active()) {
        add_to_component_list(id, hash);
    }
}
void ecs::ECS::entity_activated(ecs::ID entity_id) {
    if (entity_id == INVALID_ID || entity_id >= entities.size())
        return;
    if (!entities[entity_id].valid())
        return;
    if (!entities[entity_id].active())
        return;

    add_to_active_entities(entity_id);
    add_to_component_list(entity_id);
}

void ecs::ECS::entity_deactivated(ecs::ID entity_id) {
    if (entity_id == INVALID_ID || entity_id >= entities.size())
        return;
    if (!entities[entity_id].valid())
        return;
    if (entities[entity_id].active())
        return;
    // remove from active entities
    remove_from_active_entities(entity_id);
    remove_from_component_list(entity_id);
}
void ecs::ECS::add_to_component_list(ID id) {
    auto entity = &entities[id];
    for (auto& [hash, component] : entity->components) {
        add_to_component_list(id, hash);
    }
}
void ecs::ECS::remove_from_component_list(ID id) {
    auto entity = &entities[id];
    for (auto& [hash, irrelevant] : entity->components) {
        remove_from_component_list(id, hash);
    }
}
void ecs::ECS::add_to_component_list(ID id, ecs::Hash hash) {
    // if the component list does not exist, create it
    if (component_entity_lists.find(hash) == component_entity_lists.end()) {
        component_entity_lists[hash] = ComponentEntityList{};
        component_entity_lists[hash].set(this, hash);
    }
    component_entity_lists.at(hash).push_back(id);
}
void ecs::ECS::remove_from_component_list(ID id, ecs::Hash hash) {
    auto entity = &entities[id];
    component_entity_lists.at(hash).remove_at(entity->components[hash]->component_entity_id);
}
void ecs::ECS::add_to_active_entities(ID id) {
    active_entities.push_back(id);
}
void ecs::ECS::remove_from_active_entities(ID id) {
    active_entities.remove(id);
}

void ecs::ECS::add_system(ecs::System* system) {
    systems.push_back(system);
}
void ecs::ECS::process(double delta) {
    for (System* sys : systems) {
        sys->process(this, delta);
    }
}

