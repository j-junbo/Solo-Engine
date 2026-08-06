cmake_minimum_required(VERSION 3.20)

set(CMAKE_CHILD_DIR ${CMAKE_SOURCE_DIR})
string(APPEND CMAKE_CHILD_DIR "/../../..")

# Setting the vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CHILD_DIR}/Engine/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")
set(CMAKE_CONFIGURATION_TYPES "Engine_Debug;Engine_Release;${GAME_NAME}_Release" CACHE STRING "" FORCE)

project(${GAME_NAME} LANGUAGES CXX)

# Global defines
add_definitions(-DNOMINMAX)
add_definitions(-DGLFW_DLL)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

string(TOUPPER ${GAME_NAME} CapsName)

# Setting configurations
set(CMAKE_CXX_FLAGS_ENGINE_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_EXE_LINKER_FLAGS_ENGINE_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")

set(CMAKE_CXX_FLAGS_ENGINE_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_ENGINE_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")

set(CMAKE_CXX_FLAGS_${CapsName}_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_${CapsName}_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")


# ---------------------------------------------------------------------------
# Force all build output (regardless of config) into ./Engine/Build
# ---------------------------------------------------------------------------
file(MAKE_DIRECTORY "${CMAKE_CHILD_DIR}/Engine/Build/${GAME_NAME}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CHILD_DIR}/Engine/Build/${GAME_NAME}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CHILD_DIR}/Engine/Build/${GAME_NAME}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CHILD_DIR}/Engine/Build/${GAME_NAME}")

foreach(OUTPUTCONFIG Engine_Debug Engine_Release)
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${CMAKE_CHILD_DIR}/Engine/Build/${GAME_NAME}/${OUTPUTCONFIG}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${CMAKE_CHILD_DIR}/Engine/Build/${GAME_NAME}/${OUTPUTCONFIG}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${CMAKE_CHILD_DIR}/Engine/Build/${GAME_NAME}/${OUTPUTCONFIG}")
endforeach()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Gather sources
# ---------------------------------------------------------------------------
file(GLOB_RECURSE ENGINE_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CHILD_DIR}/Engine/Engine_Source/*.cpp"
    "${CMAKE_CHILD_DIR}/Engine/Engine_Source/*.h"
    "${CMAKE_CHILD_DIR}/Engine/Engine_Source/*.hpp"
)

file(GLOB_RECURSE SCRIPT_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/*.cpp"
    "${CMAKE_SOURCE_DIR}/*.h"
    "${CMAKE_SOURCE_DIR}/*.hpp"
)

# ---------------------------------------------------------------------------
# Engine Executable
# ---------------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${ENGINE_SOURCES} ${SCRIPT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CHILD_DIR}/Engine/Engine_Source"
)

# Preserve folder structure in the Visual Studio Solution Explorer
source_group(TREE "${CMAKE_CHILD_DIR}/Engine/Engine_Source" PREFIX "Engine" FILES ${ENGINE_SOURCES})
source_group(TREE "${CMAKE_SOURCE_DIR}" PREFIX "Scripts" FILES ${SCRIPT_SOURCES})

# Make this project the default "Set as Startup Project" in VS
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Optional: silence common MSVC warnings you've been dealing with (sign-compare, conversion)
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
endif()