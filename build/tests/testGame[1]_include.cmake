if(EXISTS "C:/Users/aaron/source/repos/Adventure/adventurev2/clonedAdv/advpacket/build/tests/testGame[1]_tests.cmake")
  include("C:/Users/aaron/source/repos/Adventure/adventurev2/clonedAdv/advpacket/build/tests/testGame[1]_tests.cmake")
else()
  add_test(testGame_NOT_BUILT testGame_NOT_BUILT)
endif()
