//
// Created by Finn Eggers on 08.06.24.
//

#ifndef ECS_RECYCLING_VECTOR_H
#define ECS_RECYCLING_VECTOR_H

#include <vector>
#include <queue>

#include "types.h"

// a vector that leaves gaps when elements are removed, and reuses those gaps for new elements
namespace ecs {

template<typename T>
struct RecyclingVector {
    std::vector<T> elements;
    std::queue<ID> free_positions;
    T default_value;

    RecyclingVector() {};

    RecyclingVector(const T& default_val) : default_value(default_val) {}

    ID push_back(const T &element) {
        if (!free_positions.empty()) {
            // Reuse a free position
            ID id = free_positions.front();
            free_positions.pop();
            elements[id] = element;
            added(id);
            return id;
        } else {
            // Append to the end
            elements.push_back(element);
            added(elements.size() - 1);
            return elements.size() - 1;
        }
    }

    void remove(const T &element) {
        // Find the element
        auto it = std::find(elements.begin(), elements.end(), element);

        // If element is found, remove it
        if (it != elements.end()) {
            ID id = std::distance(elements.begin(), it);
            elements[id] = default_value;
            free_positions.push(id);
            removed(id);
        }
    }

    void remove_at(ID id) {
        if (id < elements.size()) {
            elements[id] = default_value;
            free_positions.push(id);
            removed(id);
        }
    }

    auto begin() const {
        return elements.begin();
    }

    auto end() const {
        return elements.end();
    }

    // access operators + at()
    T &operator[](ID id) {
        return elements[id];
    }

    T &at(ID id) {
        return elements.at(id);
    }

    T &operator()(ID id) {
        return elements[id];
    }

    const T &operator[](ID id) const {
        return elements[id];
    }

    const T &at(ID id) const {
        return elements.at(id);
    }

    const T &operator()(ID id) const {
        return elements[id];
    }
    ID size() const {
        return elements.size();
    }
    void clear() {
        elements.clear();
        free_positions = std::queue<ID>();
    }

    protected:
    virtual void moved(ID old_id, ID new_id) {}
    virtual void removed(ID id) {}
    virtual void added(ID id) {}
};

} // namespace ecs

#endif // ECS_RECYCLING_VECTOR_H
