# ====================================================================
# Project:   Viotecs
# Author:    Giovanni Santini
# Mail:      giovanni.santini@proton.me
# Github:    @San7o
# License:   MIT
# ====================================================================

cmake_minimum_required(VERSION 3.16)

project(viotecs
  VERSION 1.1.0
  DESCRIPTION "Viotecs ECS for Brenta Engine"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(VIOTECS_SOURCES)
set(VIOTECS_TEST_SOURCES)
set(VIOTECS_TEST_INCLUDES)

find_program(CCACHE_PROGRAM ccache) # use ccache if available
if(CCACHE_PROGRAM)
  message(STATUS "Using ccache: ${CCACHE_PROGRAM}")
  set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

set(VIOTECS_DEFINES)

if(MSVC)
  set(VIOTECS_WARNING_FLAGS /W4 /permissive-)
  set(VIOTECS_DEBUG_FLAGS /Zi /Od)
  set(VIOTECS_RELEASE_FLAGS /O2)
  set(VIOTECS_DEBUG_DEFINES _DEBUG)
elseif(MINGW)
  set(VIOTECS_WARNING_FLAGS -Wall -Wextra -Wpedantic)
  set(VIOTECS_DEBUG_FLAGS -O0 -ggdb)
  set(VIOTECS_RELEASE_FLAGS -O3)
  set(VIOTECS_DEBUG_DEFINES DEBUG)
else()
  set(VIOTECS_WARNING_FLAGS -Wall -Wextra -Werror -Wpedantic)
  set(VIOTECS_DEBUG_FLAGS -O0 -ggdb -fsanitize=address)
  set(VIOTECS_RELEASE_FLAGS -O3)
  set(VIOTECS_DEBUG_DEFINES DEBUG)
  set(VIOTECS_DEBUG_LINK_FLAGS -fsanitize=address)
endif()

#
# Options
#

option(BUILD_SHARED_LIBS "Build shared library" ON)
option(VIOTECS_BUILD_TESTS "Build tests" OFF)

#
# Dependencies
#

# Use CPM for dependency management
include(cmake/CPM.cmake)

CPMAddPackage(
    NAME oak
    GITHUB_REPOSITORY San7o/oak
    GIT_TAG origin/main)

if(VIOTECS_BUILD_TESTS)
  CPMAddPackage(
    NAME valfuzz
    GITHUB_REPOSITORY San7o/valFuzz
    GIT_TAG origin/main)
  CPMUsePackageLock(package-lock.cmake)
endif()

#
# Targets
#

file(GLOB VIOTECS_SOURCES src/*.cpp)

if (BUILD_SHARED_LIBS)
  message(STATUS "Building shared library")
  add_library(${PROJECT_NAME} SHARED
    ${VIOTECS_SOURCES})
else()
  message(STATUS "Building static library")
  add_library(${PROJECT_NAME} STATIC
    ${VIOTECS_SOURCES})  
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
  ${PROJECT_SOURCE_DIR}/include
  ${oak_SOURCE_DIR}/include)
target_compile_definitions(${PROJECT_NAME} PRIVATE
  ${VIOTECS_DEFINES})
target_link_libraries(${PROJECT_NAME} PRIVATE
  oak)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Configuring Debug build options...")

  target_compile_definitions(${PROJECT_NAME} PRIVATE
    ${VIOTECS_DEBUG_DEFINES})
  target_compile_options(${PROJECT_NAME} PRIVATE
    ${VIOTECS_DEBUG_FLAGS}
    ${VIOTECS_WARNING_FLAGS})
  if(NOT MSVC)
    target_link_options(${PROJECT_NAME} PRIVATE
      ${VIOTECS_DEBUG_LINK_FLAGS})
  endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  message(STATUS "Configuring Release build options...")
  target_compile_options(${PROJECT_NAME} PRIVATE
    ${VIOTECS_RELEASE_FLAGS}
    ${VIOTECS_WARNING_FLAGS})
endif()

if (VIOTECS_BUILD_TESTS)
  message(STATUS "Building tests")
  
  if (NOT (valfuzz_ADDED AND oak_ADDED))
    message(ERROR "Failed to configure dependencies")
  endif()
  
  file(GLOB VIOTECS_TESTS_SOURCES tests/*.cpp)
  list(APPEND VIOTECS_TEST_INCLUDES
    include
    ${valfuzz_SOURCE_DIR}/include
    ${oak_SOURCE_DIR}/include)
  add_executable(${PROJECT_NAME}_tests
    ${VIOTECS_TESTS_SOURCES})
  target_include_directories(${PROJECT_NAME}_tests PRIVATE
    ${VIOTECS_TEST_INCLUDES})
  target_link_libraries(${PROJECT_NAME}_tests PUBLIC
    valfuzz
    oak
    ${PROJECT_NAME})

  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Configuring Debug build options...")

    target_compile_definitions(${PROJECT_NAME}_tests PRIVATE
      ${VIOTECS_DEBUG_DEFINES})
    target_compile_options(${PROJECT_NAME}_tests PRIVATE
      ${VIOTECS_DEBUG_FLAGS}
      ${VIOTECS_WARNING_FLAGS})
    if(NOT MSVC)
      target_link_options(${PROJECT_NAME}_tests PRIVATE
        ${VIOTECS_DEBUG_LINK_FLAGS})
    endif()
  elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Configuring Release build options...")
    target_compile_options(${PROJECT_NAME}_tests PRIVATE
      ${VIOTECS_RELEASE_FLAGS}
      ${VIOTECS_WARNING_FLAGS})
  endif()
  
endif()

#
# End of CMakeLists.txt
# ====================================================================
