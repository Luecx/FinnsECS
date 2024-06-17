//
// Created by Luecx on 15.06.2021.
//

#ifndef ECS_ECS_ECS_H_
#define ECS_ECS_ECS_H_

#include "component.h"
#include "component_entity_list.h"
#include "ecs_base.h"
#include "entity.h"
#include "entity_iterator.h"
#include "entity_subset.h"
#include "event.h"
#include "hash.h"
#include "system.h"
#include "types.h"
#include "vector_recycling.h"

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
    std::unordered_map<Hash, ComponentEntityList>                     component_entity_lists {};
    std::vector<Entity>                                               entities {};
    CompactVector<ID>                                                 active_entities {};

    RecyclingVector<System::Ptr>                                      systems {nullptr};
    std::unordered_map<Hash, RecyclingVector<EventListenerBase::Ptr>> event_listener {};

    // Grant Entity access to private members.
    friend Entity;
    friend ComponentEntityList;

    ECS() = default;

    // Delete copy constructor and copy assignment operator
    ECS(const ECS&) = delete;
    ECS& operator=(const ECS&) = delete;
    // Delete move constructor and move assignment operator
    ECS(ECS&&) = delete;
    ECS& operator=(ECS&&) = delete;

    virtual ~ECS() {
        destroy_all_entities();
        destroy_all_systems();
    }

    public:
    EntityID spawn(bool active = false);
    void     destroy_entity(EntityID id) override;
    void     destroy_all_entities();
    void     destroy_all_systems();

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
    void component_removed(Hash hash, EntityID id) override;
    void component_added(Hash hash, EntityID id) override;
    void entity_activated(EntityID id) override;
    void entity_deactivated(EntityID id) override;

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
    inline void emit_event(const Event& event) {
        Hash hash = get_type_hash<Event>();
        if (event_listener.find(hash) == event_listener.end())
            return;
        for (auto listener : event_listener[hash]) {
            auto l = reinterpret_cast<EventListener<Event>*>(listener.get());
            l->receive(this, event);
        }
    }

    template<typename T, typename... Args>
    SystemID create_system(Args&&... args) {
        std::shared_ptr<T> system = std::make_shared<T>(std::forward<Args>(args)...);
        ID pos = systems.push_back(system);
        return SystemID{pos};
    }
    void destroy_system(SystemID id) override {
        if (id >= systems.size())
            return;
        systems[id]->destroyed();
        systems.remove_at(id);
    }

    template<typename T, typename... Args>
    EventListenerID create_listener(Args&&... args) {
        std::shared_ptr<T> listener = std::make_shared<T>(std::forward<Args>(args)...);
        auto hash = listener->hash;

        if (event_listener.find(hash) == event_listener.end()) {
            event_listener[hash] = {nullptr};
        }
        ID pos = event_listener[hash].push_back(listener);
        return EventListenerID{pos, hash};
    }
    void destroy_listener(EventListenerID id) override {
        event_listener[id.operator Hash()].remove_at((ID)id);
    }

    void                 process(double delta);
    friend std::ostream& operator<<(std::ostream& os, const ECS& ecs1) {
        os << "All Entities: " << std::endl;
        os << "-----------------" << std::endl;
        for (const auto& entity : ecs1.entities) {
            os << "Entity ID: " << std::setw(10);
            if (entity.valid()) {
                os << entity.id() << " | Active: " << (entity.active() ? "true" : "false");
            } else {
                os << "INVALID"
                   << " | Active: -";
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
                os << "INVALID"
                   << " | Active: -";
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

}    // namespace ecs_



inline ecs::EntityID ecs::ECS::spawn(bool active) {
    entities.push_back(Entity(this));
    entities.back().entity_id = EntityID{entities.size() - 1};

    if (active) {
        entities.back().activate();
    }

    return entities.back().entity_id;
}

inline void ecs::ECS::destroy_entity(ecs::EntityID id) {
    // get the entity at the given id
    auto entity = &entities[id];

    // if active, deactivate (will notify components)
    entity->deactivate();

    // destroy the components
    entity->remove_all_components();

    // set the entity's id to INVALID_ID and the pointer to nullptr
    entity->entity_id = EntityID{INVALID_ID};
}

inline void ecs::ECS::destroy_all_entities() {
    for (Entity& entity : entities) {
        destroy_entity(entity.entity_id);
    }
    entities.clear();
}

inline void ecs::ECS::destroy_all_systems() {
    for (auto sys : systems) {
        sys->destroyed();
    }
    systems.clear();
}

inline void ecs::ECS::component_removed(ecs::Hash hash, EntityID id) {
    if (entities[id].active()) {
        remove_from_component_list(id, hash);
    }
}
inline void ecs::ECS::component_added(ecs::Hash hash, EntityID id) {
    if (entities[id].active()) {
        add_to_component_list(id, hash);
    }
}
inline void ecs::ECS::entity_activated(ecs::EntityID entity_id) {
    if (entity_id == INVALID_ID || entity_id >= entities.size())
        return;
    if (!entities[entity_id].valid())
        return;
    if (!entities[entity_id].active())
        return;

    add_to_active_entities(entity_id);
    add_to_component_list(entity_id);
}

inline void ecs::ECS::entity_deactivated(ecs::EntityID entity_id) {
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
inline void ecs::ECS::add_to_component_list(ID id) {
    auto entity = &entities[id];
    for (auto& [hash, component] : entity->components) {
        add_to_component_list(id, hash);
    }
}
inline void ecs::ECS::remove_from_component_list(ID id) {
    auto entity = &entities[id];
    for (auto& [hash, irrelevant] : entity->components) {
        remove_from_component_list(id, hash);
    }
}
inline void ecs::ECS::add_to_component_list(ID id, ecs::Hash hash) {
    // if the component list does not exist, create it
    if (component_entity_lists.find(hash) == component_entity_lists.end()) {
        component_entity_lists[hash] = ComponentEntityList{};
        component_entity_lists[hash].set(&entities, hash);
    }
    component_entity_lists.at(hash).push_back(id);
}
inline void ecs::ECS::remove_from_component_list(ID id, ecs::Hash hash) {
    auto entity = &entities[id];
    component_entity_lists.at(hash).remove_at(entity->components[hash]->component_entity_id);
}
inline void ecs::ECS::add_to_active_entities(ID id) {
    active_entities.push_back(id);
}
inline void ecs::ECS::remove_from_active_entities(ID id) {
    active_entities.remove(id);
}

inline void ecs::ECS::process(double delta) {
    for (auto sys : systems) {
        sys->process(this, delta);
    }
}




#endif    // ECS_ECS_ECS_H_
