//
// Created by Luecx on 15.06.2021.
//

#ifndef ECS_ECS_TYPES_H_
#define ECS_ECS_TYPES_H_

#include <cstdint>
#include <typeindex>

namespace ecs {


struct Entity;

struct ECS;

struct ComponentEntityList;

/**
 * @brief Forward declaration of the System structure.
 */
struct System;

/**
 * @brief Template structure for iterating over entities with specific component types.
 *
 * @tparam RTypes The component types required for the iteration.
 */
template<typename... RTypes>
struct EntityIterator;

/**
 * @brief Template structure for a subset of entities with specific component types.
 *
 * @tparam RTypes The component types that define the subset.
 */
template<typename... RTypes>
struct EntitySubSet;

/**
 * @brief Type alias for entity IDs.
 */
typedef uint32_t ID;

/**
 * @brief Type alias for component hashes using std::type_index.
 */
typedef std::type_index Hash;

/**
 * @brief Constant representing an invalid entity ID.
 */
#define INVALID_ID ID(-1)

}    // namespace ecs

#endif    // ECS_ECS_TYPES_H_
