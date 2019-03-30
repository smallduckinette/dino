cmake_minimum_required(VERSION 3.7)

project(dino)

set(CMAKE_CXX_FLAGS "-DBOOST_LOG_DYN_LINK -O0 -ggdb -std=c++17 -W -Wall -Werror -pedantic")

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Boost REQUIRED COMPONENTS log program_options)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)


include_directories(${SDL2_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${JSONCPP_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

add_library(gltf SHARED buffer.cpp parser.cpp)

add_executable(dino main.cpp shader.cpp buffer.cpp)
target_link_libraries(dino gltf ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${JSONCPP_LIBRARIES} ${Boost_LIBRARIES})

add_executable(tests test_main.cpp test_gltf.cpp)
target_link_libraries(tests gltf ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${JSONCPP_LIBRARIES} ${Boost_LIBRARIES})