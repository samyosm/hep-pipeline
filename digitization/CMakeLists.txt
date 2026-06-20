project(Digitization)

find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

set(SOURCES
    src/main.cc
)

add_executable(digitize ${SOURCES})

target_include_directories(digitize PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(digitize PRIVATE
    hepp_utils
    ${ROOT_LIBRARIES}
)
