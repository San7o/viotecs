cmake_minimum_required(VERSION 3.2.9)
project("viotecs")

include(cmake/CPM.cmake) # <3

CPMAddPackage(
    NAME oak
    GITHUB_REPOSITORY San7o/oak
    GIT_TAG v1.0.2
    DOWNLOAD_ONLY True
)
CPMAddPackage(
    NAME valfuzz
    GITHUB_REPOSITORY San7o/valFuzz
    GIT_TAG v1.0.0
    DOWNLOAD_ONLY True
)
CPMUsePackageLock(package-lock.cmake)

option(VIOTECS_BUILD_TESTS "Build tests" ON)

set(CMAKE_CXX_STANDARD 23)
set(VIOTECS_SOURCES)
set(VIOTECS_TEST_SOURCES)
set(VUITECS_TEST_INCLUDES)

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

if (VIOTECS_BUILD_TESTS)
    add_executable(viotecs_tests ${VIOTECS_SOURCES}
            ${VIOTECS_TESTS_SOURCES})
    target_include_directories(viotecs_tests PRIVATE
            ${VIOTECS_TEST_INCLUDES})
endif()
