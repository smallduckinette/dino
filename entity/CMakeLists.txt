add_library(entity SHARED entityfactory.cpp)
target_include_directories(entity SYSTEM PUBLIC ${JSONCPP_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(entity ${JSONCPP_LIBRARIES})
