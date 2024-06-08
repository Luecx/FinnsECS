//
// Created by Finn Eggers on 31.05.24.
//

#ifndef F3D_ECS_BASE_H
#define F3D_ECS_BASE_H

#include "types.h"

namespace ecs{

struct ECSBase {
    virtual void component_removed(Hash hash, ID id) = 0;

    virtual void component_added(Hash hash, ID id) = 0;

    virtual void entity_activated(ID id) = 0;

    virtual void entity_deactivated(ID id) = 0;

    virtual void destroy_entity(ID id) = 0;
};

}

#endif    // F3D_ECS_BASE_H
