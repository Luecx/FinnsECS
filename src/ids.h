//
// Created by Finn Eggers on 09.06.24.
//

#ifndef ECS_IDS_H
#define ECS_IDS_H

#include "types.h"

namespace ecs {

struct EntityID {
    ID id = INVALID_ID;
    operator ID() const { return id; }
    operator ID&() { return id; }
};

struct SystemID {
    ID id = INVALID_ID;
    operator ID() const { return id; }
    operator ID&() { return id; }
};

struct EventListenerID {
    ID   id   = INVALID_ID;
    Hash hash = INVALID_HASH;
    operator ID() const { return id; }
    operator ID&() { return id; }
    operator Hash() const { return hash; }
    operator Hash&() { return hash; }
};

} // namespace ecs

#endif // ECS_IDS_H
