cmake_minimum_required(VERSION 3.14)

project(checkers_backend LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

add_executable(checkers_backend
  main.cpp
  server.h server.cpp
  roombase.h roombase.cpp
  participant.h participant.cpp
  packettypekeeperservice.h packettypekeeperservice.cpp
  humanvshumanroom.h humanvshumanroom.cpp
  humanvsrobotroom.h humanvsrobotroom.cpp
  humanparticipant.h humanparticipant.cpp
  robotparticipant.h robotparticipant.cpp
  gamemodel.h gamemodel.cpp
  board.h board.cpp
  validatorbase.h validatorbase.cpp
  validatorpawn.h validatorpawn.cpp
  validatordame.h validatordame.cpp
  useablepiecefinder.h useablepiecefinder.cpp
  ToastTypeEnum.h
  positionevaluator.h positionevaluator.cpp
  robotstepcalculator.h robotstepcalculator.cpp
  evaluatorbase.h evaluatorbase.cpp
  evaluatorpawn.h evaluatorpawn.cpp
  evaluatordame.h evaluatordame.cpp
)

target_link_libraries(checkers_backend Qt${QT_VERSION_MAJOR}::Core Qt6::Network)

include(GNUInstallDirs)
install(TARGETS checkers_backend
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
