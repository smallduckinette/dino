add_library(gltf SHARED asset.cpp builder.cpp)
target_include_directories(gltf SYSTEM PUBLIC ${JSONCPP_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(gltf core adh ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${JSONCPP_LIBRARIES})
