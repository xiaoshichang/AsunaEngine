
FILE(GLOB_RECURSE AsunaEngineSources ${CMAKE_SOURCE_DIR}/AsunaEngine "*.cpp" "*.c" "*.h")
add_library(AsunaEngine ${AsunaEngineSources})
TARGET_COMPILE_DEFINITIONS(AsunaEngine PUBLIC ASUNAENGINE_EXPORTS )


target_link_libraries(AsunaEngine assimp::assimp opengl32)

