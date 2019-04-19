add_library(adh SHARED camera.cpp context.cpp material.cpp node.cpp plaincolor.cpp primitive.cpp shader.cpp texture.cpp texturecolor.cpp transform.cpp)

target_link_libraries(adh ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
