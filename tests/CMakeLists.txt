include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
add_executable(tests test_main.cpp test_body.cpp test_gltf.cpp test_interpolator.cpp)
target_link_libraries(tests gltf adh shootengine ${Boost_LIBRARIES} stdc++fs)
