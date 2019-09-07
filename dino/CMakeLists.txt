include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
add_executable(dino controller.cpp main.cpp world.cpp)
target_link_libraries(dino gltf adh ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${JSONCPP_LIBRARIES} ${Boost_LIBRARIES} stdc++fs)
