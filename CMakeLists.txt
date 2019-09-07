cmake_minimum_required(VERSION 3.7)

project(dino)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
set(CMAKE_CXX_FLAGS "-DBOOST_LOG_DYN_LINK -O0 -ggdb -W -Wall -Werror -pedantic")

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Boost REQUIRED COMPONENTS log program_options)
find_package(Bullet REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
pkg_check_modules(SDL2IMAGE SDL2_image)

include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(core)
add_subdirectory(adh)
add_subdirectory(gltf)
add_subdirectory(tests)
add_subdirectory(dino)
add_subdirectory(shoot)
