add_executable(Test_Math Test_Math.cpp Test_Math.cpp Test_Math.cpp)
add_test(NAME Test_Math COMMAND Test_Math)
target_include_directories(Test_Math PUBLIC ${CMAKE_SOURCE_DIR})