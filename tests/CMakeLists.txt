project(UnitTests)

add_subdirectory(catch2)

set(TEST_MODULES "test_boolean.cpp" "test_decimal.cpp" "test_integer.cpp")

set(TEST_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/catch2/src")

add_executable(${PROJECT_NAME})

target_sources(
  ${PROJECT_NAME}
  PUBLIC
  PUBLIC FILE_SET CXX_MODULES FILES ${TEST_MODULES})

target_include_directories(${PROJECT_NAME} PUBLIC ${TEST_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain nhtypes)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/catch2/extras)

enable_testing()
include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_NAME})

add_custom_target(
  AllTests ALL
  COMMAND ${CMAKE_CTEST_COMMAND} ${CMAKE_BINARY_DIR}
  DEPENDS ${PROJECT_NAME}
  COMMENT "Running tests")
