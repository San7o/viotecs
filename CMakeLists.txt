cmake_minimum_required(VERSION 3.16)
project("viotecs")

option(BUILD_SHARED_LIBS "Build shared library" ON)
option(VIOTECS_BUILD_TESTS "Build tests" OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(VIOTECS_SOURCES)
set(VIOTECS_TEST_SOURCES)
set(VIOTECS_TEST_INCLUDES)

include(cmake/CPM.cmake) # <3

CPMAddPackage(
    NAME oak
    GITHUB_REPOSITORY San7o/oak
    GIT_TAG v1.0.2
    DOWNLOAD_ONLY True
)

if(VIOTECS_BUILD_TESTS)
  
  CPMAddPackage(
    NAME valfuzz
    GITHUB_REPOSITORY San7o/valFuzz
    GIT_TAG v1.0.0
    DOWNLOAD_ONLY True
  )
  CPMUsePackageLock(package-lock.cmake)
  
endif()

list(APPEND VIOTECS_SOURCES
    src/world.cpp
)

if (VIOTECS_BUILD_TESTS AND valfuzz_ADDED AND oak_ADDED)
    list(APPEND VIOTECS_TESTS_SOURCES
        tests/component_test.cpp
        tests/entity_test.cpp
        tests/resource_test.cpp
        tests/world_test.cpp
        tests/systems_test.cpp
        ${valfuzz_SOURCE_DIR}/src/valfuzz.cpp
        ${oak_SOURCE_DIR}/src/oak.cpp
    )
    list(APPEND VIOTECS_TEST_INCLUDES
        include
        ${valfuzz_SOURCE_DIR}/include
        ${oak_SOURCE_DIR}/include
    )
endif()

if (BUILD_SHARED_LIBS)
  add_library(${PROJECT_NAME} SHARED ${VIOTECS_SOURCES})
else()
  add_library(${PROJECT_NAME} STATIC ${VIOTECS_SOURCES})  
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
  ${PROJECT_SOURCE_DIR}/include ${oak_SOURCE_DIR}/include)


if (VIOTECS_BUILD_TESTS)
    add_executable(viotecs_tests ${VIOTECS_SOURCES}
            ${VIOTECS_TESTS_SOURCES})
    target_include_directories(viotecs_tests PRIVATE
            ${VIOTECS_TEST_INCLUDES})
endif()
