project(table_game)

if(TESTS STREQUAL "yes")
    enable_testing()
endif()

cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(lib)

message(TESTS: ${TESTS})

if(TESTS STREQUAL "yes")
    add_subdirectory(tests)
endif()

set (SRC_LIST main.cpp)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} tg-core)
