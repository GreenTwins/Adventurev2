cmake_minimum_required(VERSION 3.15)

project(game)


add_library(game
    src/Character.cpp
    src/Enemy.cpp
    src/Game.cpp
    src/Player.cpp
    src/Skills.cpp
    src/SQLCONN.cpp
)

include_directories("C:/Program Files/Microsoft SQL Server/Client SDK/ODBC/170/SDK/Include")
link_directories("C:/Program Files/Microsoft SQL Server/Client SDK/ODBC/170/SDK/Lib/x64")

target_link_libraries(game
    odbc32.lib
    odbccp32.lib
    "C:/Program Files/Microsoft SQL Server/Client SDK/ODBC/170/SDK/Lib/x64/msodbcsql17.lib"
)

target_include_directories(game
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(game PUBLIC cxx_std_17)
