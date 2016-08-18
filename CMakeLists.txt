project(table_game)

if(TESTS STREQUAL "yes")
    enable_testing()
endif()

cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
add_definitions(-Wall -Wextra)

add_subdirectory(lib)
include_directories(${CMAKE_SOURCE_DIR}/lib)

message(TESTS: ${TESTS})

if(TESTS STREQUAL "yes")
    add_subdirectory(tests)
endif()

set (SRC_LIST main.cpp)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} tg-core)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc ALL
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif(DOXYGEN_FOUND)
