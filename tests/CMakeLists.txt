cmake_minimum_required(VERSION 3.28.1)

set(ALGOLAB_TEST_NAME algolabtests)
set(LLVM_CPP_PATH "/opt/homebrew/Cellar/llvm/19.1.7/include/c++/v1")

project(${ALGOLAB_TEST_NAME})
set(CMAKE_CXX_STANDARD 20)

# -fexperimental-library is required for jthread in C++20
# -I${LLVM_CPP_PATH} is required for latest installed libc++ headers 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g -O0 -Wall -fexperimental-library -I${LLVM_CPP_PATH}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")

# Use google test for unit tests
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

include_directories(BEFORE "${LLVM_CPP_PATH}")
include_directories(BEFORE "/opt/homebrew/Cellar/fmt/10.1.1/include")
include_directories(BEFORE "/opt/homebrew/Cellar/nlohmann-json/3.11.3/include")
include_directories((BEFORE "../build/_deps/googletest-src/googletest/include/gtest"))
include_directories((BEFORE "../algolab"))

add_library(libfmt SHARED IMPORTED)

set(SOURCES_TEST main.cpp merge_sort_test.cpp bubble_sort_test.cpp parameterized_sort_test.cpp)

find_package(fmt)

# Testing with google test
enable_testing()
add_executable(${ALGOLAB_TEST_NAME} ${SOURCES_TEST})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Building in Debug mode")
    target_compile_definitions(${ALGOLAB_TEST_NAME} PRIVATE DEBUG_MODE)
    target_compile_options(${ALGOLAB_TEST_NAME} PRIVATE -g -O0 -stdlib=libc++ -fexperimental-library)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Building in Release mode")
    target_compile_options(${ALGOLAB_TEST_NAME} PRIVATE -O3)
endif()

target_link_libraries(${ALGOLAB_TEST_NAME} algolab fmt::fmt GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(${ALGOLAB_TEST_NAME})