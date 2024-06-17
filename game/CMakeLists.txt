cmake_minimum_required(VERSION 3.15)

project(game)

add_library(game src/Character.cpp)
add_library(game src/Enemy.cpp)
add_library(game src/Game.cpp)
add_library(game src/Player.cpp)
add_library(game src/Skills.cpp)
add_library(game src/SQLCONN.cpp)

target_include_directories(${PROJECT_NAME}
PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_compile_features(game PUBLIC cxx_std_20)
