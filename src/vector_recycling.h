//
// Created by Finn Eggers on 08.06.24.
//

#ifndef ECS_VECTOR_RECYCLING_H
#define ECS_VECTOR_RECYCLING_H

#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>

#include "types.h"

// a vector that leaves gaps when elements are removed, and reuses those gaps for new elements
namespace ecs {

template<typename T>
struct RecyclingVector {
    std::vector<T> elements_;
    std::queue<ID> free_positions_;
    T default_value_;

    RecyclingVector() {};

    RecyclingVector(const T& default_val) : default_value_(default_val) {}

    ID push_back(const T &element) {
        if (!free_positions_.empty()) {
            // Reuse a free position
            ID id = free_positions_.front();
            free_positions_.pop();
            elements_[id] = element;
            added(id);
            return id;
        } else {
            // Append to the end
            elements_.push_back(element);
            added(elements_.size() - 1);
            return elements_.size() - 1;
        }
    }

    void remove(const T &element) {
        // Find the element
        auto it = std::find(elements_.begin(), elements_.end(), element);

        // If element is found, remove it
        if (it != elements_.end()) {
            ID id = std::distance(elements_.begin(), it);
            elements_[id] = default_value_;
            free_positions_.push(id);
            removed(id);
        }
    }

    void remove_at(ID id) {
        if (id < elements_.size()) {
            elements_[id] = default_value_;
            free_positions_.push(id);
            removed(id);
        }
    }

    auto begin() const {
        return elements_.begin();
    }

    auto end() const {
        return elements_.end();
    }

    // access operators + at()
    T &operator[](ID id) {
        return elements_[id];
    }

    T &at(ID id) {
        return elements_.at(id);
    }

    T &operator()(ID id) {
        return elements_[id];
    }

    const T &operator[](ID id) const {
        return elements_[id];
    }

    const T &at(ID id) const {
        return elements_.at(id);
    }

    const T &operator()(ID id) const {
        return elements_[id];
    }

    ID size() const {
        return elements_.size();
    }

    void clear() {
        elements_.clear();
        free_positions_ = std::queue<ID>();
    }

    protected:
    virtual void moved(ID old_id, ID new_id) {}
    virtual void removed(ID id) {}
    virtual void added(ID id) {}
};

} // namespace ecs

#endif    // ECS_VECTOR_RECYCLING_H
