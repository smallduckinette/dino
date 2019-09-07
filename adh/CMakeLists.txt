include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_library(adh SHARED animation.cpp camera.cpp context.cpp node.cpp primitive.cpp rtclock.cpp shader.cpp texture.cpp transform.cpp)

target_include_directories(adh SYSTEM PUBLIC ${JSONCPP_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${SDL_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
target_link_libraries(adh ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${SDL_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
