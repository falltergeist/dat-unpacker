cmake_minimum_required(VERSION 3.8)
project(dat-unpacker VERSION 0.0.5)

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

aux_source_directory(src SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    VERSION ${PROJECT_VERSION}
)

target_link_libraries(${PROJECT_NAME} PRIVATE ZLIB::ZLIB)
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
