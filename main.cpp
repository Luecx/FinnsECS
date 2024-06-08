#include <iostream>

#include "src/include.h"

int main() {

    ecs::ECS ecs{};
    std::cout << ecs << std::endl;
    ecs::ID id = ecs.spawn();
    ecs::ID id2 = ecs.spawn();
    ecs::ID id3 = ecs.spawn();
    ecs[id].activate();
    std::cout << ecs << std::endl;
    ecs.destroy_entity(id);
    std::cout << ecs << std::endl;

    return 0;
}
