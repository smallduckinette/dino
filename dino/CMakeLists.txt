include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_executable(dino controller.cpp main.cpp world.cpp)

target_include_directories(dino PUBLIC ${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${JSONCPP_INCLUDE_DIRS})
target_link_libraries(dino gltf adh ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${Boost_LIBRARIES} stdc++fs)
