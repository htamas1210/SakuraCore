# Require a modern version of CMake 
cmake_minimum_required(VERSION 3.15)

# Define the Core Library project
project(SakuraCore LANGUAGES CXX)

# ------------------------------------------------------------------------------
# Default Build Type Configuration
# ------------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting default build type to 'Debug'")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Enforce C++20 globally
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ------------------------------------------------------------------------------
# Output Directory Configuration
# ------------------------------------------------------------------------------
set(OUTPUT_DIR_SUFFIX "$<CONFIG>-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${OUTPUT_DIR_SUFFIX}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${OUTPUT_DIR_SUFFIX}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${OUTPUT_DIR_SUFFIX}")

# ------------------------------------------------------------------------------
# Dependencies
# ------------------------------------------------------------------------------
# 1. SDL3
set(SDL_TESTS OFF CACHE BOOL "Disable SDL3 tests" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "Disable SDL3 examples" FORCE)
add_subdirectory(libs/sdl3)

# 2. spdlog
set(SPDLOG_BUILD_COMPILED ON CACHE BOOL "Build spdlog as a compiled library" FORCE)
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "Disable spdlog examples" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "Disable spdlog tests" FORCE)
add_subdirectory(libs/spdlog)

# 3. ImGui (Standard UI for debug menus/core rendering)
add_subdirectory(libs/imgui)

# 4. Catch2 (Only in Debug)
if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR NOT CMAKE_BUILD_TYPE)
    add_subdirectory(libs/catch2)
endif()

# ------------------------------------------------------------------------------
# Source Gathering
# ------------------------------------------------------------------------------
file(GLOB_RECURSE SAKURA_CORE_SRCS "SakuraCore/src/*.cpp" "SakuraCore/src/*.h")

# ------------------------------------------------------------------------------
# SakuraCore STATIC Library Target
# ------------------------------------------------------------------------------
add_library(SakuraCore STATIC ${SAKURA_CORE_SRCS})

target_include_directories(SakuraCore PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "libs/spdlog/include"
    "libs/imgui"
    "libs/imgui/misc"
    "libs/imgui/backends"
    "libs/sdl3/include"
)

target_link_libraries(SakuraCore PUBLIC
    ImGui               
    SDL3::SDL3          
    spdlog::spdlog      
)

# ------------------------------------------------------------------------------
# Tests Executable Target
# ------------------------------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR NOT CMAKE_BUILD_TYPE)
    # The tests executable just needs its own test.cpp file
    add_executable(tests "SakuraCore/test/test.cpp")

    # Link Catch2 and our newly created SakuraCore library!
    # Because SakuraCore is PUBLIC above, 'tests' automatically gets SDL3, ImGui, etc.
     target_include_directories(tests PRIVATE
        "SakuraCore/src"
        "libs/spdlog/include"
        "libs/imgui"
        "libs/imgui/misc"
        "libs/imgui/backends"
        "libs/sdl3/include"
    )

    target_link_libraries(tests PRIVATE 
        Catch2::Catch2WithMain
        SakuraCore
    )
endif()

# ------------------------------------------------------------------------------
# Platform & Configuration Filters
# ------------------------------------------------------------------------------
if(WIN32)
    target_compile_definitions(SakuraCore PUBLIC PLATFORM_WINDOWS)
    target_link_options(SakuraCore PUBLIC "/NODEFAULTLIB:MSVCRT")
    
    if(TARGET tests)
        target_compile_definitions(tests PRIVATE PLATFORM_WINDOWS)
        target_link_options(tests PRIVATE "/NODEFAULTLIB:MSVCRT")
    endif()

elseif(UNIX AND NOT APPLE)
    target_compile_definitions(SakuraCore PUBLIC PLATFORM_LINUX)
    target_link_libraries(SakuraCore PUBLIC dl pthread)
    
    if(TARGET tests)
        target_compile_definitions(tests PRIVATE PLATFORM_LINUX)
        target_link_libraries(tests PRIVATE dl pthread)
    endif()
endif()

# Debug vs Release definitions
target_compile_definitions(SakuraCore PUBLIC
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:RELEASE>
)

if(TARGET tests)
    target_compile_definitions(tests PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:RELEASE>
    )
endif()
