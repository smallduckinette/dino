add_library(adh SHARED material.cpp node.cpp primitive.cpp texture.cpp)

target_link_libraries(adh ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
