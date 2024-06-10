//
// Created by Luecx on 15.06.2021.
//

#ifndef ECS_ECS_TYPES_H_
#define ECS_ECS_TYPES_H_

#include <cstdint>
#include <typeindex>

namespace ecs {

struct Entity;

struct ECSBase;
struct ECS;

struct ComponentEntityList;

struct System;

template<typename... RTypes>
struct EntityIterator;

template<typename... RTypes>
struct EntitySubSet;

typedef std::size_t     ID;
typedef std::type_index Hash;

#define INVALID_ID   ID(-1)
#define INVALID_HASH Hash(typeid(void))


}    // namespace ecs_

#endif    // ECS_ECS_TYPES_H_
