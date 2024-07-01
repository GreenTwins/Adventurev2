cmake_minimum_required(VERSION 3.15)

add_executable(testGame test_gamefunction.cpp)

target_link_libraries(testGame gtest gtest_main game)

target_compile_features(testGame PUBLIC cxx_std_17)

include(GoogleTest)

gtest_discover_tests(testGame)



