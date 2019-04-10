add_library(gltf SHARED accessor.cpp buffer.cpp bufferview.cpp nodecache.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(gltf adh stdc++fs)
