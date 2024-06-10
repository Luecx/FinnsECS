//
// Created by Finn Eggers on 09.06.24.
//

#ifndef ECS_IDS_H
#define ECS_IDS_H

#include "types.h"

namespace ecs {

struct EntityID {
    ID id = INVALID_ID;
    operator ID() const;
    operator ID&();
};

struct ComponentID {
    ID id = INVALID_ID;
    operator ID() const;
    operator ID&();
};

struct SystemID {
    ID id = INVALID_ID;
    operator ID() const;
    operator ID&();
};

struct EventListenerID {
    ID   id   = INVALID_ID;
    Hash hash = INVALID_HASH;
    operator ID() const;
    operator ID&();
    operator Hash() const;
    operator Hash&();
};

}

#endif    // ECS_IDS_H
