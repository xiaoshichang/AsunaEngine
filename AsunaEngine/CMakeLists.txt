cmake_minimum_required(VERSION 3.20)
project(AsunaEngine)

set(CMAKE_CXX_STANDARD 17)

find_package(assimp CONFIG REQUIRED)

add_subdirectory(AsunaEngine)
add_subdirectory(AsunaPlayer)
add_subdirectory(AsunaEditor)
add_subdirectory(AsunaEngineUnitTest)
