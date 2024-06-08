//
// Created by Luecx on 15.06.2021.
//

#ifndef ECS_ECS_ECS_H_
#define ECS_ECS_ECS_H_

#include "component.h"
#include "component_entity_list.h"
#include "ecs_base.h"
#include "entity.h"
#include "event.h"
#include "hash.h"
#include "iterator.h"
#include "system.h"
#include "types.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <vector>

namespace ecs {

/**
 * @brief Represents the Entity Component System (ECS) manager.
 *
 * The ECS class manages entities, components, and systems, providing an interface
 * to create entities, add/remove components, and process systems.
 */
struct ECS : public ECSBase {
    std::unordered_map<Hash, ComponentEntityList>             component_entity_lists {};
    std::vector<Entity>                                       entities {};
    SelfArrangingList<ID>                                     active_entities {};

    std::vector<System*>                                      systems {};
    std::unordered_map<Hash, std::vector<EventBaseListener*>> event_listener {};

    // Grant Entity access to private members.
    friend Entity;
    friend ComponentEntityList;

    virtual ~ECS() {
        destroy_all_entities();
        destroy_all_systems();
    }

    public:
    ID   spawn(bool active = false);
    void destroy_entity(ID id);
    void destroy_all_entities();
    void destroy_all_systems();

    // operators to get an entity from an id
    Entity& operator[](ID id) {
        return entities[id];
    }
    Entity operator[](ID id) const {
        return entities[id];
    }
    Entity& at(ID id) {
        return entities.at(id);
    }
    Entity at(ID id) const {
        return entities.at(id);
    }
    Entity& operator()(ID id) {
        return entities[id];
    }
    Entity operator()(ID id) const {
        return entities[id];
    }

    private:
    // listeners to functions applied onto the entities
    void component_removed(Hash hash, ID id) override;
    void component_added(Hash hash, ID id) override;
    void entity_activated(ID id) override;
    void entity_deactivated(ID id) override;

    // functions to manage the lists
    void add_to_component_list(ID entity);
    void remove_from_component_list(ID entity);
    void add_to_component_list(ID entity, Hash hash);
    void remove_from_component_list(ID entity, Hash hash);
    void add_to_active_entities(ID entity);
    void remove_from_active_entities(ID entity);

    public:
    template<typename K, typename... R>
    inline EntitySubSet<K, R...> each() {
        auto  hash = K::hash();
        auto* ids  = &component_entity_lists[hash].elements;
        return EntitySubSet<K, R...> {ids, &entities};
    }

    template<typename K, typename... R>
    inline ID first() {
        for (ID id : component_entity_lists[K::hash()].entities) {
            if (entities[id].has<K, R...>()) {
                return id;
            }
        }
        return INVALID_ID;
    }

    template<typename Event>
    inline void add_event_listener(EventListener<Event>* listener) {
        Hash hash = get_type_hash<Event>();
        if (event_listener.find(hash) == event_listener.end()) {
            event_listener[hash] = {};
        }
        event_listener[hash].push_back(listener);
    }

    template<typename Event>
    inline void emit_event(const Event& event) {
        Hash hash = get_type_hash<Event>();
        if (event_listener.find(hash) == event_listener.end())
            return;
        for (EventBaseListener* listener : event_listener[hash]) {
            auto l = reinterpret_cast<EventListener<Event>*>(listener);
            l->receive(event);
        }
    }

    void                 add_system(System* system);
    void                 process(double delta);
    friend std::ostream& operator<<(std::ostream& os, const ECS& ecs1) {
        os << "All Entities: " << std::endl;
        os << "-----------------" << std::endl;
        for (const auto& entity : ecs1.entities) {
            os << "Entity ID: " << std::setw(10);
            if (entity.valid()) {
                os << entity.id() << " | Active: " << (entity.active() ? "true" : "false");
            } else {
                os << "INVALID" << " | Active: -";
            }
            os << std::endl;
        }
        os << std::endl;

        os << "Active Entities: " << std::endl;
        os << "-----------------" << std::endl;
        for (const auto& id : ecs1.active_entities) {
            os << "Entity ID: " << std::setw(10);
            if (id != INVALID_ID) {
                os << id << " | Active: true";
            } else {
                os << "INVALID" << " | Active: -";
            }
            os << std::endl;
        }
        os << std::endl;

        os << "Component Entity List: " << std::endl;
        os << "-----------------------" << std::endl;
        for (const auto& pair : ecs1.component_entity_lists) {
            os << "Component Hash: " << std::setw(20) << pair.first.hash_code() << std::endl;
            os << "Entities: " << std::endl;
            for (const auto& id : pair.second) {
                os << std::setw(10);
                if (id != INVALID_ID) {
                    os << id << " | Active: " << (ecs1.entities[id].active() ? "true" : "false");
                } else {
                    os << "INVALID_ID | Active: -";
                }
                os << std::endl;
            }
            os << "-----------------------" << std::endl;
        }
        os << std::endl;

        return os;
    }
};

}    // namespace ecs

#endif    // ECS_ECS_ECS_H_
