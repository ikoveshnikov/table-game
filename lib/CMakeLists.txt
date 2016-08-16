file(GLOB SRC_FILES *.cpp)
file(GLOB HEADERS *.h)

add_library(
    tg-core
    ${SRC_FILES}
    ${HEADERS}
)
