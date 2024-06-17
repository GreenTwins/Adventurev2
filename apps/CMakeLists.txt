cmake_minimum_required(VERSION 3.15)

project(main)

add_executable(main main.cpp)

target_link_libraries(main
PRIVATE game)


target_compile_definitions(main PUBLIC cxx_std_20)
set_target_properties(main PROPERTIES  
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin 
)