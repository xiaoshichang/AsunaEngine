
FILE(GLOB_RECURSE AsunaPlayerSources ${CMAKE_SOURCE_DIR}/AsunaPlayer "*.cpp" "*.c" "*.h")
add_executable(AsunaPlayer ${AsunaPlayerSources})

target_include_directories(AsunaPlayer PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(AsunaPlayer AsunaEngine)

install(DIRECTORY "${PROJECT_SOURCE_DIR}/AsunaPlayer/Assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")


