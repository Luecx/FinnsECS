#ifndef ECS_ECS_ITERATOR_H_
#define ECS_ECS_ITERATOR_H_

#include "entity.h"
#include "types.h"

#include <iostream>
#include <vector>

namespace ecs {

template<typename... RTypes>
struct EntityIterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = Entity;
    using pointer           = value_type*;    // or also value_type*
    using reference         = value_type&;    // or also value_type&

    EntityIterator(std::vector<ID>::iterator id_iter,
                   std::vector<ID>::iterator id_end,
                   std::vector<Entity>* entity_packs)
        : m_id_iter(id_iter), m_id_end(id_end), m_entity_packs(entity_packs) {

        advance_to_next_valid();
    }

    reference operator*() const {
        return (*m_entity_packs)[*m_id_iter];
    }

    pointer operator->() {
        return &(*m_entity_packs)[*m_id_iter];
    }

    // Prefix increment
    EntityIterator& operator++() {
        ++m_id_iter;
        advance_to_next_valid();
        return *this;
    }

    // Postfix increment
    EntityIterator operator++(int) {
        EntityIterator temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const EntityIterator& rhs) const {
        return m_id_iter == rhs.m_id_iter;
    }

    bool operator!=(const EntityIterator& rhs) const {
        return m_id_iter != rhs.m_id_iter;
    }

    private:
    std::vector<ID>::iterator m_id_iter;
    std::vector<ID>::iterator m_id_end;
    std::vector<Entity>* m_entity_packs;

    void advance_to_next_valid() {

        while (   m_id_iter != m_id_end
               && !(*m_entity_packs)[*m_id_iter].template has<RTypes...>()) {

            // if id is IVALID_ID, skip
            if (*m_id_iter == INVALID_ID) {
                ++m_id_iter;
                continue;
            }

            ++m_id_iter;
        }

    }
};

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

} // namespace ecs

#endif // ECS_ECS_ITERATOR_H_
