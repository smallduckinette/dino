add_library(core SHARED json_utils.cpp)
target_include_directories(core SYSTEM PUBLIC ${JSONCPP_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(core ${JSONCPP_LIBRARIES})
