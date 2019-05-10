cmake_minimum_required(VERSION 3.7)

project(dino)

enable_testing()
set(CMAKE_CXX_FLAGS "-DBOOST_LOG_DYN_LINK -O0 -ggdb -std=c++17 -W -Wall -Werror -pedantic")

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Boost REQUIRED COMPONENTS log program_options)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
pkg_check_modules(SDL2IMAGE SDL2_image)


include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${JSONCPP_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

add_subdirectory(adh)
add_subdirectory(gltf)

add_executable(dino controller.cpp main.cpp world.cpp)
target_link_libraries(dino gltf adh ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${JSONCPP_LIBRARIES} ${Boost_LIBRARIES} stdc++fs)

add_executable(tests test_main.cpp test_gltf.cpp test_interpolator.cpp)
target_link_libraries(tests gltf adh ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${JSONCPP_LIBRARIES} ${Boost_LIBRARIES} stdc++fs)
