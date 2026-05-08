project(EventGeneration)

add_executable(gen_events src/main.cc)

# HepMC3
find_package(HepMC3 REQUIRED)
target_link_libraries(gen_events PRIVATE HepMC3::HepMC3)

# Pythia8
find_program(PYTHIA8_CONFIG pythia8-config REQUIRED)

execute_process(
    COMMAND ${PYTHIA8_CONFIG} --cxxflags
    OUTPUT_VARIABLE PYTHIA8_CXXFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND ${PYTHIA8_CONFIG} --ldflags --libs
    OUTPUT_VARIABLE PYTHIA8_LDFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

separate_arguments(PYTHIA8_CXXFLAGS)
separate_arguments(PYTHIA8_LDFLAGS)

target_compile_options(gen_events PRIVATE ${PYTHIA8_CXXFLAGS})
target_link_libraries(gen_events PRIVATE ${PYTHIA8_LDFLAGS})

# OpenMp
find_package(OpenMP REQUIRED)

if(OpenMP_CXX_FOUND)
    target_link_libraries(gen_events PUBLIC OpenMP::OpenMP_CXX)
endif()

# Others
target_link_libraries(gen_events PRIVATE hepp_utils)

