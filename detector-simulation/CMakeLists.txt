project(DetectorSimulation)

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

find_package(HepMC3 REQUIRED)
include_directories(${HEPMC3_INCLUDE_DIRS})

set(SOURCES
    src/main.cc
    src/DetectorConstruction.cc
    src/ActionInitialization.cc
    src/PrimaryGeneratorAction.cc
    src/SteppingAction.cc
    src/EventAction.cc
    src/RunAction.cc
)

add_executable(simulate_detection ${SOURCES})

target_include_directories(simulate_detection PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(simulate_detection PRIVATE 
    ${Geant4_LIBRARIES} 
    ${HEPMC3_LIBRARIES}
    hepp_utils
)
