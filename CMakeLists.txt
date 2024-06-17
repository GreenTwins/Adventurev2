cmake_minimum_required(VERSION 3.15)

project(
    VscodeCmakeGame
    VERSION 0.1
    DESCRIPTION "Adventure game v2"
)

include(FetchContent)

FetchContent_Declare(
    googletest  
    GIT_REPOSITORY https://github.com/google/googletest.git  
    GIT_TAG release-1.10.0
)

FetchContent_MakeAvailable(googletest)
add_subdirectory(apps)
add_subdirectory(game)