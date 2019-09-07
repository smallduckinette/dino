include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_executable(shoot main.cpp)

target_include_directories(shoot PUBLIC ${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${JSONCPP_INCLUDE_DIRS})
target_link_libraries(shoot gltf adh ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${Boost_LIBRARIES} stdc++fs)
