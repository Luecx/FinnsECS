//
// Created by Finn Eggers on 08.06.24.
//

#ifndef ECS_VECTOR_COMPACT_H
#define ECS_VECTOR_COMPACT_H

#include <vector>

#include "types.h"

// a normal vector more or less but when removing, we simply shift the last element to the position of the removed one
// using templates to generalise for storage type
namespace ecs {

template<typename T>
struct CompactVector {
    std::vector<T> elements;

    void push_back(const T &element) {
        elements.push_back(element);
    }
    void remove(const T &element) {
        // if the element is at the end, we can simply pop it
        if (element == elements.back()) {
            elements.pop_back();
            removed(element);
            return;
        }

        // check position of the element
        auto it = std::find(elements.begin(), elements.end(), element);

        // if it doesnt exist, exit here
        if (it == elements.end()) {
            return;
        }

        // otherwise we need to find the element and move the last element to its position
        if (it != elements.end()) {
            // get the id of the last element and the element to be removed
            auto last_id = elements.size() - 1;
            auto element_id = static_cast<ID>(std::distance(elements.begin(), it));

            removed(element_id);

            // move
            elements[element_id] = elements.back();
            elements.pop_back();

            moved(last_id, element_id);
        }
    }
    void remove_at(ID id) {
        // if the element is at the end, we can simply pop it
        if (id == elements.size() - 1) {
            elements.pop_back();
            removed(id);
            return;
        }

        // otherwise we need to find the element and move the last element to its position
        if (id < elements.size()) {
            // get the id of the last element and the element to be removed
            auto last_id = elements.size() - 1;

            removed(id);

            // move
            elements[id] = elements.back();
            elements.pop_back();

            moved(last_id, id);
        }
    }
    auto begin() const { return elements.begin(); }
    auto end() const { return elements.end(); }

    // access operators + at()
    T &operator[](ID id) { return elements[id]; }
    T &at(ID id) { return elements.at(id); }
    T &operator()(ID id) { return elements[id]; }
    const T &operator[](ID id) const { return elements[id]; }
    const T &at(ID id) const { return elements.at(id); }
    const T &operator()(ID id) const { return elements[id]; }

    ID size() const { return elements.size(); }
    void clear() { elements.clear(); }

    protected:

    virtual void moved(ID old_id, ID new_id) {};
    virtual void removed(ID id) {};
    virtual void added(ID id) {};
};

}    // namespace ecs_

#endif    // ECS_VECTOR_COMPACT_H
