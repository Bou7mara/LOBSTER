cmake_minimum_required(VERSION 3.25)
project(lobster LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.7.1
)
FetchContent_MakeAvailable(Catch2)

add_library(lobster_lib INTERFACE)
target_include_directories(lobster_lib INTERFACE ${CMAKE_SOURCE_DIR}/src)

add_executable(lobster_main src/main.cpp)
target_link_libraries(lobster_main PRIVATE lobster_lib)

enable_testing()

add_executable(lobster_tests tests/test_main.cpp)
target_link_libraries(lobster_tests PRIVATE lobster_lib Catch2::Catch2WithMain)

include(Catch2/extras/Catch.cmake  OPTIONAL)
if(COMMAND catch_discover_tests)
    catch_discover_tests(lobster_tests)
endif()
