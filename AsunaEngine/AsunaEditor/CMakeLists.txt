

FILE(GLOB_RECURSE AsunaEditorSources ${CMAKE_SOURCE_DIR}/AsunaEditor "*.cpp" "*.c" "*.h")
add_executable(AsunaEditor ${AsunaEditorSources} Component/MeshRenderEditor.cpp Component/MeshRenderEditor.h)

target_include_directories(AsunaEditor PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(AsunaEditor AsunaEngine)
