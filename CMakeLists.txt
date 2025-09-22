cmake_minimum_required(VERSION 4.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
  SafeTypes
  VERSION 1.0
  DESCRIPTION "Introduces Integers/Decimals and Booleans with runtime error checking"
  LANGUAGES CXX)

set(MODULES "src/types.cpp" "src/common.cpp" "src/boolean.cpp" "src/decimals.cpp" "src/integers.cpp" "src/type_traits.cpp")

add_library(${PROJECT_NAME})

target_sources(
  ${PROJECT_NAME}
  PUBLIC
  PUBLIC FILE_SET CXX_MODULES FILES ${MODULES})

if(BUILD_TESTS)
    add_compile_definitions(ENABLE_TESTABLE_ASSERTIONS)
  add_subdirectory(tests)
endif()

