cmake_minimum_required(VERSION 3.20)
project(AsunaEngine)

set(CMAKE_CXX_STANDARD 17)

find_package(imgui CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)


message("-- [Asuna Engine] CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    add_definitions(-D_Debug_)
ELSEIF(${CMAKE_BUILD_TYPE} MATCHES Release)
    add_definitions(-D_Release_)
ELSE()
    message( FATAL_ERROR "Unknown build type!" )
ENDIF()


add_subdirectory(AsunaEngine)
add_subdirectory(AsunaPlayer)
add_subdirectory(AsunaEditor)


include(CTest)
add_subdirectory(AsunaEngineUnitTest)
