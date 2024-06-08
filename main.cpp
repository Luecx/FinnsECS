#include "src/include.h"

#include <iostream>

struct C1 : public ecs::ComponentBaseOf<C1> {
    int  value = 0;

    void component_removed() override {
        std::cout << "C1 removed" << std::endl;
    }
    void entity_activated() override {
        std::cout << "C1 activated" << std::endl;
    }
    void entity_deactivated() override {
        std::cout << "C1 deactivated" << std::endl;
    }
    void other_component_added(ecs::Hash hash) override {
        std::cout << "C1 other component added" << std::endl;
    }
    void other_component_removed(ecs::Hash hash) override {
        std::cout << "C1 other component removed" << std::endl;
    }
};

struct C2 : public ecs::ComponentBaseOf<C2> {
    int  value = 0;

    void component_removed() override {
        std::cout << "C2 removed" << std::endl;
    }
    void entity_activated() override {
        std::cout << "C2 activated" << std::endl;
    }
    void entity_deactivated() override {
        std::cout << "C2 deactivated" << std::endl;
    }
    void other_component_added(ecs::Hash hash) override {
        std::cout << "C2 other component added" << std::endl;
    }
    void other_component_removed(ecs::Hash hash) override {
        std::cout << "C2 other component removed" << std::endl;
    }
};

struct C3 : public ecs::ComponentBaseOf<C3> {
    int  value = 0;

    void component_removed() override {
        std::cout << "C3 removed" << std::endl;
    }
    void entity_activated() override {
        std::cout << "C3 activated" << std::endl;
    }
    void entity_deactivated() override {
        std::cout << "C3 deactivated" << std::endl;
    }
    void other_component_added(ecs::Hash hash) override {
        std::cout << "C3 other component added" << std::endl;
    }
    void other_component_removed(ecs::Hash hash) override {
        std::cout << "C3 other component removed" << std::endl;
    }
};

int main() {

    ecs::ECS ecs {};
    std::cout << ecs << std::endl;
    ecs::ID id  = ecs.spawn();
    ecs::ID id2 = ecs.spawn();
    ecs::ID id3 = ecs.spawn();

    // assign components
    ecs[id].assign<C1>();
    ecs[id].assign<C2>();
    ecs[id].assign<C3>();
    ecs[id2].assign<C1>();
    ecs[id2].assign<C2>();
    ecs[id3].assign<C3>();
    ecs[id3].assign<C2>();

    ecs[id].activate();
    ecs[id2].activate();
    ecs[id3].activate();

    std::cout << ecs << std::endl;

    // check manually which id holds C1 and C2:
    std::cout << "E1: " << ecs[id].has<C1>() << " " << ecs[id].has<C2>() << " " << ecs[id].has<C3>()
              << std::endl;
    std::cout << "E2: " << ecs[id2].has<C1>() << " " << ecs[id2].has<C2>() << " "
              << ecs[id2].has<C3>() << std::endl;
    std::cout << "E3: " << ecs[id3].has<C1>() << " " << ecs[id3].has<C2>() << " "
              << ecs[id3].has<C3>() << std::endl;

    for (auto& entity : ecs.each<C1, C2>()) {
        std::cout << entity.id() << std::endl;
    }

    return 0;
}
