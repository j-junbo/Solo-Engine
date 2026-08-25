cmake_minimum_required(VERSION 3.20)

# Setting the vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/Engine/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")
set(CMAKE_CONFIGURATION_TYPES "Engine_Debug;Engine_Release" CACHE STRING "" FORCE)


project(GameEngine LANGUAGES CXX)

# Global defines
add_definitions(-DNOMINMAX)
add_definitions(-DGLFW_DLL)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# Setting configurations
set(CMAKE_CXX_FLAGS_ENGINE_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_EXE_LINKER_FLAGS_ENGINE_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")

set(CMAKE_CXX_FLAGS_ENGINE_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_ENGINE_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")


# ---------------------------------------------------------------------------
# Force all build output (regardless of config) into ./Engine/Build
# ---------------------------------------------------------------------------
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/Engine/Build/Engine")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Engine/Build/Engine")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Engine/Build/Engine")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Engine/Build/Engine")

foreach(OUTPUTCONFIG Engine_Debug Engine_Release)
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${CMAKE_SOURCE_DIR}/Engine/Build/Engine/${OUTPUTCONFIG}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${CMAKE_SOURCE_DIR}/Engine/Build/Engine/${OUTPUTCONFIG}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${CMAKE_SOURCE_DIR}/Engine/Build/Engine/${OUTPUTCONFIG}")
    set(PDB_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${CMAKE_SOURCE_DIR}/Engine/Build/Engine/${OUTPUTCONFIG}")
endforeach()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Gather sources
# ---------------------------------------------------------------------------
file(GLOB_RECURSE ENGINE_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/Engine/Engine_Source/*.cpp"
    "${CMAKE_SOURCE_DIR}/Engine/Engine_Source/*.h"
    "${CMAKE_SOURCE_DIR}/Engine/Engine_Source/*.hpp"
)

list(REMOVE_ITEM ENGINE_SOURCES "${CMAKE_SOURCE_DIR}/Engine/Engine_Source/main.cpp")

# ---------------------------------------------------------------------------
# Engine Executable + DLL
# ---------------------------------------------------------------------------
add_executable(${PROJECT_NAME}Exec WIN32 "${CMAKE_SOURCE_DIR}/Engine/Engine_Source/main.cpp")

add_library(${PROJECT_NAME} SHARED ${ENGINE_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_dependencies(${PROJECT_NAME}Exec ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/Engine/Engine_Source"
)

# Preserve folder structure in the Visual Studio Solution Explorer
source_group(TREE "${CMAKE_SOURCE_DIR}/Engine/Engine_Source" PREFIX "Engine" FILES ${ENGINE_SOURCES})

# Make this project the default "Set as Startup Project" in VS
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME}Exec)

target_compile_options(${PROJECT_NAME} PRIVATE /W4)
target_compile_options(${PROJECT_NAME}Exec PRIVATE /W4)

target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Engine_Debug>:/Zi>)
target_link_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Engine_Debug>:/DEBUG>)

# ---------------------------------------------------------------------------
# vcpkg sources
# ---------------------------------------------------------------------------

find_package(GLEW CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_package(Freetype CONFIG REQUIRED)
find_package(rttr CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
	GLEW::GLEW
	glfw
	glm::glm
	imgui::imgui
	Freetype::Freetype
	implot::implot
    imguizmo::imguizmo
    RTTR::Core
)


# ---------------------------------------------------------------------------
# other sources
# ---------------------------------------------------------------------------

target_link_libraries(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/Engine/External_Libs/fmod/lib/x64/fmod_vc.lib"
)

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/Engine/External_Libs/fmod/inc"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Engine/External_Libs/fmod/lib/x64/fmod.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)

# ---------------------------------------------------------------------------
# Scripts Executable
# ---------------------------------------------------------------------------

# Find the names of all different Game Scripts
file(GLOB AbsoluteNameList
	"${CMAKE_SOURCE_DIR}/Engine/Game_Scripts/*"
)

set(NameList)
foreach(entry ${AbsoluteNameList})
	get_filename_component(name "${entry}" NAME)
	list(APPEND NameList "${name}")
endforeach()

# call their individual cmakelists.txt
foreach(GameName ${NameList})
    execute_process(COMMAND cmake -B ../../../Engine/Build/${GameName} -G "Visual Studio 17 2022" -A x64 -DGAME_NAME=${GameName}
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Engine/Game_Scripts/${GameName}")
endforeach()


# ---------------------------------------------------------------------------
# Creating Shortcuts
# ---------------------------------------------------------------------------
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/Engine/Solutions")

# main engine first
execute_process(COMMAND powershell -NoProfile -Command
    "$s = New-Object -ComObject WScript.Shell; $lnk = $s.CreateShortcut('${CMAKE_SOURCE_DIR}/Engine/Solutions/GameEngine.lnk'); $lnk.TargetPath = '${CMAKE_SOURCE_DIR}/Engine/Build/Engine/GameEngine.sln'; $lnk.Save()"
)

foreach(GameName ${NameList})
    execute_process(COMMAND powershell -NoProfile -Command
        "$s = New-Object -ComObject WScript.Shell; $lnk = $s.CreateShortcut('${CMAKE_SOURCE_DIR}/Engine/Solutions/${GameName}.lnk'); $lnk.TargetPath = '${CMAKE_SOURCE_DIR}/Engine/Build/${GameName}/${GameName}.sln'; $lnk.Save()"
    )
endforeach()
