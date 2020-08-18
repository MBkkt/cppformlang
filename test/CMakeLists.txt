cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(cppformlang_tests LANGUAGES CXX)

# ---- Options ----

option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)
option(TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage(
  NAME doctest
  GITHUB_REPOSITORY onqtam/doctest
  GIT_TAG 2.3.7
)

if(TEST_INSTALLED_VERSION)
  find_package(cppformlang REQUIRED)
else()
  CPMAddPackage(NAME cppformlang SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
endif()

CPMAddPackage(
  NAME Format.cmake
  GITHUB_REPOSITORY TheLartians/Format.cmake
  VERSION 1.6
  OPTIONS # enable cmake formatting
          "FORMAT_CHECK_CMAKE ON"
          # skip CPM.cmake
          "CMAKE_FORMAT_EXCLUDE cmake/CPM.cmake"
)

# ---- Create binary ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
add_executable(cppformlang_tests ${sources})
target_link_libraries(cppformlang_tests doctest cppformlang)

set_target_properties(cppformlang_tests PROPERTIES CXX_STANDARD 17)

# enable compiler warnings
if(NOT TEST_INSTALLED_VERSION)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(cppformlang PUBLIC -Wall -pedantic -Wextra -Werror)
  elseif(MSVC)
    target_compile_options(cppformlang PUBLIC /W4 /WX)
    target_compile_definitions(cppformlang_tests PUBLIC DOCTEST_CONFIG_USE_STD_HEADERS)
  endif()
endif()

# ---- Add cppformlang_tests ----

enable_testing()

# Note: doctest and similar testing frameworks can automatically configure CMake tests For other
# testing frameworks add the tests target instead: ADD_TEST(cppformlang_tests cppformlang_tests)

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(cppformlang_tests)

# ---- code coverage ----

if(ENABLE_TEST_COVERAGE)
  target_compile_options(cppformlang PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(cppformlang PUBLIC -fprofile-arcs -ftest-coverage)
endif()
