#ifndef ECS_ECS_ENTITY_H_
#define ECS_ECS_ENTITY_H_

#include "component.h"
#include "ecs_base.h"
#include "hash.h"
#include "types.h"

#include <memory>
#include <unordered_map>

namespace ecs {

/**
 * @brief Represents an entity in the ECS (Entity Component System).
 *
 * The Entity class manages a collection of components and provides methods
 * to add, remove, and access these components. It also supports basic operations
 * like checking for component existence and entity comparison.
 */
struct Entity {
    private:
    // Unique identifier for the entity.
    ID entity_id;
    // Container for the entity's components.
    std::unordered_map<Hash, ComponentPtr> components;
    // Pointer to the ECS that owns this entity.
    ECSBase* ecs;
    // Checks if its active or inactive.
    bool m_active = false;

    // Grant ECS access to private members.
    friend ECS;
    friend ComponentEntityList;

    public:

    Entity(ECSBase* p_ecs)
        : entity_id(INVALID_ID)
        , ecs(p_ecs) {}

    virtual ~Entity() {
        remove_all_components();
    }

    template<typename T>
    bool has() const {
        Hash hash = T::hash();
        return components.find(hash) != components.end();
    }

    template<typename T, typename V, typename... Types>
    bool has() const {
        return has<T>() && has<V, Types...>();
    }

    template<typename T>
    std::shared_ptr<T> get() {
        Hash hash = get_type_hash<T>();
        if (components.find(hash) != components.end()) {
            return std::static_pointer_cast<T>(components.at(hash));
        }
        return nullptr;
    }

    template<typename T, typename... Args>
    inline void assign(Args&&... args) {
        auto component = std::make_shared<T>(std::forward<Args>(args)...);
        Hash hash      = T::hash();

        // If the component already exists, remove it first
        if (has<T>()) {
            remove_component<T>();
        }

        // Add the new component
        components[hash] = component;
        ecs->component_added(hash, id());

        // notify all other components that a new component was added
        for (auto& [hash, comp] : components) {
            // dont do it for itself
            if (hash == component->get_hash())
                continue;

            // call the other_component_added function
            comp->other_component_added(hash);
            component->other_component_added(hash);
        }

        // inform the component that it was added to an active entity
        if (m_active) {
            component->entity_activated();
        }
    }

    template<typename T>
    inline void remove_component() {
        if (!has<T>())
            return;

        Hash hash = get_type_hash<T>();
        components[hash]->component_removed();
        components.erase(hash);
        ecs->component_removed(hash, id());
    }

    inline void remove_all_components() {
        for (auto& pair : components) {
            ecs->component_removed(pair.first, id());
        }
        for (auto& pair : components) {
            pair.second->component_removed();
        }

        components.clear();
    }

    inline void destroy() {
        ecs->destroy_entity(this->entity_id);
    }

    ID id() const {
        return entity_id;
    }

    bool valid() const {
        return entity_id != INVALID_ID;
    }

    bool active() const {
        return m_active;
    }

    void activate() {
        if (!m_active) {
            m_active = true;
            ecs->entity_activated(this->entity_id);
            for (auto& [hash, component] : components) {
                component->entity_activated();
            }
        }
    }

    void deactivate() {
        if (m_active) {
            m_active = false;
            ecs->entity_deactivated(this->entity_id);
            for (auto& [hash, component] : components) {
                component->entity_deactivated();
            }
        }
    }

    void set_active(bool val) {
        if (val) {
            activate();
        } else {
            deactivate();
        }
    }

    // Comparison operators for the Entity class.
    bool operator==(const Entity& rhs) const {
        return entity_id == rhs.entity_id;
    }
    bool operator!=(const Entity& rhs) const {
        return !(rhs == *this);
    }
    bool operator<(const Entity& rhs) const {
        return entity_id < rhs.entity_id;
    }
    bool operator>(const Entity& rhs) const {
        return rhs < *this;
    }
    bool operator<=(const Entity& rhs) const {
        return !(rhs < *this);
    }
    bool operator>=(const Entity& rhs) const {
        return !(*this < rhs);
    }

    // Stream output to display hash, id, valid, active all nicely in new rows indented
    friend std::ostream& operator<<(std::ostream& os, const Entity& entity) {
        os << "Entity ID: " << entity.entity_id << std::endl;
        os << "\tValid: " << entity.valid() << std::endl;
        os << "\tActive: " << entity.active() << std::endl;
        return os;
    }
};

}    // namespace ecs

#endif    // ECS_ECS_ENTITY_H_