cmake_minimum_required(VERSION 4.4.2)
project(LOBSTER VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wshadow)
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose build type: Debug Release RelWithDebInfo" FORCE)
endif()

if(NOT MSVC)
    add_compile_options("$<$<CONFIG:Debug>:-fsanitize=address,undefined>")
    add_link_options("$<$<CONFIG:Debug>:-fsanitize=address,undefined>")
    add_compile_options("$<$<CONFIG:Release>:-march=native>")
endif()

add_library(lobster_core INTERFACE)
target_include_directories(lobster_core INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_executable(lobster_bin src/main.cpp)
target_link_libraries(lobster_bin PRIVATE lobster_core)
set_target_properties(lobster_bin PROPERTIES OUTPUT_NAME "lobster")

include(CTest)
if(BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.6.0
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(lobster_tests
        tests/test_main.cpp
    )

    target_link_libraries(lobster_tests PRIVATE
        lobster_core
        Catch2::Catch2WithMain
    )

    include(Catch)
    catch_discover_tests(lobster_tests)
endif()
