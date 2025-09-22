cmake_minimum_required(VERSION 4.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
  SafeTypes
  VERSION 1.0
  DESCRIPTION "Introduces Integers/Decimals and Booleans with runtime error checking"
  LANGUAGES CXX)

set(TYPE_MODULES "src/base.cpp" "src/boolean.cpp" "src/decimals.cpp" "src/integers.cpp" "src/type_traits.cpp")

set(TEST_INCLUDE_DIRS "${HELIUM_EXTERNAL_DIR}/catch2/src")

add_library(${PROJECT_NAME})

target_sources(
  ${PROJECT_NAME}
  PUBLIC
  PUBLIC FILE_SET CXX_MODULES FILES ${TEST_MODULES})
