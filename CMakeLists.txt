cmake_minimum_required(VERSION 3.14)

project(JeuVieRomaineSimul LANGUAGES CXX)

file(TO_CMAKE_PATH "$ENV{IMAGINEPP_ROOT}/CMake" p)
find_package(Imagine REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(JeuVieRomaineSimul
  main.cpp parametres.h
  case.cpp case.h
  joueur.cpp joueur.h
  carte.cpp carte.h
  plateau.cpp plateau.h
  paquet.cpp paquet.h
  game.cpp game.h
)

ImagineUseModules(JeuVieRomaineSimul Graphics)
