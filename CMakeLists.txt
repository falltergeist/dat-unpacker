project(datfile)

find_package(ZLIB REQUIRED)
if(NOT ZLIB_FOUND)
          message(FATAL_ERROR "Zlib library not found")
endif(NOT ZLIB_FOUND)
include_directories(${ZLIB_INCLUDE_DIRS})

cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
set(CMAKE_BUILD_TYPE Debug)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${ZLIB_LIBRARIES})

