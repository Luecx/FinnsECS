#ifndef ECS_ECS_ENTITY_H_
#define ECS_ECS_ENTITY_H_

#include "component.h"
#include "ecs_base.h"
#include "hash.h"
#include "types.h"
#include "ids.h"

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
    EntityID entity_id;
    // Container for the entity's components.
    std::unordered_map<Hash, ComponentPtr> components{};
    // Pointer to the ECS that owns this entity.
    ECSBase* ecs;
    // Checks if its active or inactive.
    bool m_active = false;

    // Grant ECS access to private members.
    friend ECS;
    friend ComponentEntityList;

    public:

    // Copy constructor deleted but manual move constructor
    Entity(const Entity& other) = delete;

    // Move constructor
    Entity(Entity&& other) noexcept
        : entity_id(other.entity_id)
        , components(std::move(other.components))
        , ecs(other.ecs)
        , m_active(other.m_active) {
        other.entity_id.id = INVALID_ID;
        other.ecs = nullptr;
        other.m_active = false;
    }

    // Copy assignment operator
    Entity& operator=(const Entity& other) = delete;

    // Move assignment operator
    Entity& operator=(Entity&& other) noexcept {
        if (this == &other)
            return *this;

        remove_all_components();

        entity_id = other.entity_id;
        components = std::move(other.components);
        ecs = other.ecs;
        m_active = other.m_active;

        other.entity_id.id = INVALID_ID;
        other.ecs = nullptr;
        other.m_active = false;
        return *this;
    }

    Entity(ECSBase* p_ecs = nullptr)
        : entity_id{INVALID_ID}
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
    T* get() {
        Hash hash = get_type_hash<T>();
        if (components.find(hash) != components.end()) {
            return static_cast<T*>(components.at(hash).get());
        }
        return nullptr;
    }

    template<typename T, typename... Args>
    inline ComponentID assign(Args&&... args) {
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        Hash hashing   = T::hash();

        // assign ecs to the component
        component->ecs = reinterpret_cast<ECS*>(ecs);
        // assign id
        component->component_id = ComponentID{entity_id, hashing};

        // If the component already exists, remove it first
        if (has<T>()) {
            remove_component<T>();
        }

        // Add the new component
        components[hashing] = std::move(component);
        ecs->component_added(hashing, id());

        // notify all other components that a new component was added
        for (auto& [hash, comp] : components) {
            // dont do it for itself
            if (hash == hashing)
                continue;

            // call the other_component_added function
            comp->other_component_added(hash);
            components[hashing]->other_component_added(hash);
        }

        // inform the component that it was added to an active entity
        if (m_active) {
            component->entity_activated();
        }

        // return the component id
        return components[hashing]->component_id;
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

    EntityID id() const {
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

}    // namespace ecs_

#endif    // ECS_ECS_ENTITY_H_
