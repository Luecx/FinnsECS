cmake_minimum_required(VERSION 3.27)
project(ECS)

set(CMAKE_CXX_STANDARD 17)

add_executable(ECS main.cpp
        src/component_entity_list.h
        src/ecs.cpp
        src/component_entity_list.cpp)
