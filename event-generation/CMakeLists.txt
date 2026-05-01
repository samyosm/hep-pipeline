project(PythiaLab)

execute_process(COMMAND pythia8-config --prefix OUTPUT_VARIABLE PYTHIA8_PREFIX OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND pythia8-config --includedir OUTPUT_VARIABLE PYTHIA8_INCLUDE_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND pythia8-config --libs OUTPUT_VARIABLE PYTHIA8_LIBS_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)

string(REPLACE "-l" "" PYTHIA8_LIBS_CLEAN "${PYTHIA8_LIBS_FLAGS}")
separate_arguments(PYTHIA8_LIBS_LIST NATIVE_COMMAND "${PYTHIA8_LIBS_CLEAN}")

add_executable(gen_events src/main.cc)

target_include_directories(gen_events PRIVATE ${PYTHIA8_INCLUDE_DIR})

target_link_libraries(gen_events PRIVATE ${PYTHIA8_LIBS_LIST} hepp_utils z dl)

message(STATUS "Using Pythia8 from: ${PYTHIA8_PREFIX}")

