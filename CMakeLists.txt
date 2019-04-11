cmake_minimum_required(VERSION 3.13)
project(comp371raytracer)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)

find_package(glm REQUIRED)
#find_package(pthread REQUIRED)
find_package(X11 REQUIRED)


set(SOURCE_FILES main.cpp Scene.cpp Camera.cpp Object.cpp Light.cpp Sphere.cpp Plane.cpp Ray.cpp Image.cpp Mesh.cpp Triangle.cpp)

add_executable(comp371raytracer ${SOURCE_FILES})

target_link_libraries(comp371raytracer glm pthread X11)