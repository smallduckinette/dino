include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_executable(shoot main.cpp physicsystem.cpp)

target_include_directories(shoot SYSTEM PUBLIC ${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${JSONCPP_INCLUDE_DIRS} ${BULLET_INCLUDE_DIRS})
target_link_libraries(shoot gltf adh ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${Boost_LIBRARIES} ${BULLET_LIBRARIES} stdc++fs)
