//
// Created by Luecx on 15.06.2021.
//

#ifndef ECS_ECS_SYSTEM_H_
#define ECS_ECS_SYSTEM_H_

#include "ecs_base.h"
#include "types.h"

#include <memory>

namespace ecs {

struct System {
    using Ptr = std::shared_ptr<System>;

    friend struct ECS;

    protected:
    virtual void process(ECS* ecs, double delta) = 0;
    virtual void destroyed() {};
};

}    // namespace ecs_

#endif    // ECS_ECS_SYSTEM_H_
