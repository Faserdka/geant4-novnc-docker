cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(OpticalDetector)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})

file(GLOB SOURCES src/*.cc)
file(GLOB HEADERS include/*.hh)

add_executable(optical_detector optical_detector.cc ${SOURCES} ${HEADERS})
target_include_directories(optical_detector PRIVATE include)
target_link_libraries(optical_detector ${Geant4_LIBRARIES})
