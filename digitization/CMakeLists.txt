project(Digitization)

find_package(HepMC3 REQUIRED)
include_directories(${HEPMC3_INCLUDE_DIRS})

set(SOURCES
    src/main.cc
)

add_executable(digitize ${SOURCES})

target_include_directories(digitize PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(digitize PRIVATE
    hepp_utils
)
