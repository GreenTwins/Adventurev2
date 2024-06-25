cmake_minimum_required(VERSION 3.15)

project(
    VscodeCmakeGame
    VERSION 0.1
    DESCRIPTION "Adventure game v2"
)

set(CMAKE_CXX_STANDARD           17)
set(CMAKE_CXX_STANDARD_REQUIRED  ON)
set(CMAKE_CXX_EXTENSIONS         OFF)

#include(FetchContent)

#FetchContent_Declare(
   # googletest  
  #  GIT_REPOSITORY https://github.com/google/googletest.git  
  #  GIT_TAG main  # Use the main branch for the latest version
#)

#FetchContent_MakeAvailable(googletest)

# Custom cmd to rmv build dir 
if(WIN32)
    execute_process(COMMAND powershell -Command "if(Test-Path build) {Remove-Item -Recurse -Force build}")
else()
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR})
endif()

add_subdirectory(game)
add_subdirectory(apps)  # Ensure this line is uncommented

message("Include directories: ${CMAKE_INCLUDE_PATH}")
message("Library directories: ${CMAKE_LIBRARY_PATH}")
# Set output dir for exe
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
