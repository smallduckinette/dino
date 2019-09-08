include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_library(shootengine SHARED graphicsystem.cpp physicsystem.cpp body.cpp)
target_include_directories(shootengine SYSTEM PUBLIC ${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${JSONCPP_INCLUDE_DIRS} ${BULLET_INCLUDE_DIRS})
target_link_libraries(shootengine core entity gltf adh ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${Boost_LIBRARIES} ${BULLET_LIBRARIES} stdc++fs)

add_executable(shoot main.cpp)
target_link_libraries(shoot shootengine)
