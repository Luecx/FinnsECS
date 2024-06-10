#include "include.h"

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

struct Ball : public ecs::ComponentBaseOf<Ball> {
    float  pos = 0;
    float  vel = 0;

    // constructor
    Ball(float pos, float vel = 0.f) : pos(pos), vel(vel) {}
};

struct EnergyLoss : public ecs::ComponentBaseOf<EnergyLoss> {
    float loss = 0.0;

    EnergyLoss(float loss) : loss(loss) {}
};

struct Collision {};

struct CollisionListener : public ecs::EventListener<Collision> {
    void receive(ecs::ECS* ecs, const Collision& event) {
        // go through all the balls and if they are negative in position, adjust
        for(auto& ent: ecs->each<Ball>()) {
            auto comp = ent.get<Ball>();
            if(comp->pos < 0) {
                comp->pos = -comp->pos;
                comp->vel = -comp->vel;
            }

            // if it has energy loss, apply it using v2^2 = v1^2 * (1 - loss)
            if(ent.has<EnergyLoss>()) {
                auto loss = ent.get<EnergyLoss>();
                comp->vel = std::sqrt(comp->vel * comp->vel * (1 - loss->loss));
            }
        }
    };
};

struct Gravity : public ecs::System{
    void process(ecs::ECS* ecs, double delta) {
        for(auto ent: ecs->each<Ball>()) {
            auto comp = ent.get<Ball>();

            comp->vel -= 9.81 * delta;
            comp->pos += comp->vel * delta;

            // if pos < 0, bounce
            if(comp->pos < 0) {
                ecs->emit_event(Collision{});
            }
        }
    }
};

struct Display : public ecs::System{
    void process(ecs::ECS* ecs, double delta) {
        for(auto ent: ecs->each<Ball>()) {
            auto comp = ent.get<Ball>();
            std::cout << "Ball Position: " << comp->pos << std::endl;
        }
    }
};

int main() {

    // cretae an ecs, add a ball entity with energy loss and a system to proces. add another system to show the position of the ball
    ecs::ECS ecs;
    auto id = ecs.spawn();
    ecs[id].assign<EnergyLoss>(0.1f);
    ecs[id].assign<Ball>(10, 0);
    ecs[id].activate();

    ecs.create_system<Gravity>();
    ecs.create_system<Display>();
    ecs.create_listener<CollisionListener>();

    for(int i = 0; i < 1000; i++) {
        ecs.process(0.01);
    }


    return 0;
}
