include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
add_executable(tests test_main.cpp test_gltf.cpp test_interpolator.cpp)
target_link_libraries(tests gltf adh ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${JSONCPP_LIBRARIES} ${Boost_LIBRARIES} stdc++fs)
