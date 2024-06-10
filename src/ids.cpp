//
// Created by Finn Eggers on 09.06.24.
//

#include "ids.h"
ecs::EntityID::operator ID() const { return id; }
ecs::EntityID::operator ID&() { return id; }

ecs::ComponentID::operator ID() const { return id; }
ecs::ComponentID::operator ID&() { return id; }

ecs::SystemID::operator ID() const { return id; }
ecs::SystemID::operator ID&() { return id; }

ecs::EventListenerID::operator ID() const { return id; }
ecs::EventListenerID::operator ID&() { return id; }
ecs::EventListenerID::operator Hash() const { return hash; }
ecs::EventListenerID::operator Hash&() { return hash; }