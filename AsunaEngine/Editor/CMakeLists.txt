

FILE(GLOB_RECURSE AsunaEditorSources ${CMAKE_SOURCE_DIR}/AsunaEditor "*.cpp" "*.c" "*.h")
add_executable(AsunaEditor ${AsunaEditorSources})

target_include_directories(AsunaEditor PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(AsunaEditor AsunaEngine)

install(DIRECTORY "${PROJECT_SOURCE_DIR}/Assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")