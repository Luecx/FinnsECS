//
// Created by Finn Eggers on 31.05.24.
//

#ifndef F3D_ECS_BASE_H
#define F3D_ECS_BASE_H

#include "types.h"
#include "ids.h"

namespace ecs{

struct ECSBase {
    virtual void component_removed(Hash, EntityID) = 0;
    virtual void component_added(Hash, EntityID) = 0;

    virtual void entity_activated(EntityID) = 0;
    virtual void entity_deactivated(EntityID) = 0;

    virtual void destroy_entity(EntityID) = 0;
    virtual void destroy_system(SystemID) = 0;
    virtual void destroy_listener(EventListenerID) = 0;
};

}

#endif    // F3D_ECS_BASE_H
