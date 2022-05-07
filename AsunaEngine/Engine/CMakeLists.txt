
FILE(GLOB_RECURSE AsunaEngineSources ${CMAKE_SOURCE_DIR}/AsunaEngine "*.cpp" "*.c" "*.h")
add_library(AsunaEngine ${AsunaEngineSources} Graphics/Abstract/RenderPass/RenderPassMgr.cpp Graphics/Abstract/RenderPass/RenderPassMgr.h)
TARGET_COMPILE_DEFINITIONS(AsunaEngine PUBLIC ASUNAENGINE_EXPORTS )

target_link_libraries(AsunaEngine imgui::imgui assimp::assimp opengl32 dxgi d3d11 d3dcompiler)

