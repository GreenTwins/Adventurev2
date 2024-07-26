cmake_minimum_required(VERSION 3.15)

project(main)

add_executable(main main.cpp)


target_include_directories(main
    PRIVATE
    ${CMAKE_SOURCE_DIR}/game/include
)

target_link_libraries(main PRIVATE game)

target_compile_features(main PUBLIC cxx_std_17)

set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)