cmake_minimum_required(VERSION 3.29)
project(raytracer VERSION 1.0.0 LANGUAGES C CXX)

file(GLOB_RECURSE sources src/*.cpp src/*.h)

add_executable(${PROJECT_NAME} ${sources})