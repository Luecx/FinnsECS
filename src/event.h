//
// Created by Luecx on 16.06.2021.
//

#ifndef ECS_ECS_EVENT_H_
#define ECS_ECS_EVENT_H_

#include "hash.h"
#include "types.h"
#include "ids.h"

#include <memory>
#include "types.h"
#include "ids.h"

namespace ecs {

struct EventListenerBase {
    using Ptr = std::shared_ptr<EventListenerBase>;
};

template<typename Event>
struct EventListener : public EventListenerBase {
    const Hash hash = get_type_hash<Event>();

    virtual void receive(ECS* ecs, const Event& event) = 0;
};

}    // namespace ecs_
#endif    // ECS_ECS_EVENT_H_
