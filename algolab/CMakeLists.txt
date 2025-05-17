cmake_minimum_required(VERSION 3.28.1)

set(MY_LIB_NAME algolab)
set(LLVM_CPP_PATH "/opt/homebrew/Cellar/llvm/19.1.7/include/c++/v1")

project(${MY_LIB_NAME})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -fexperimental-library is required for jthread in C++20
# -I${LLVM_CPP_PATH} is required for latest installed libc++ headers 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g -O0 -Wall -pedantic -Werror -fexperimental-library -I${LLVM_CPP_PATH}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")

include_directories(BEFORE "${LLVM_CPP_PATH}")
include_directories(BEFORE "/opt/homebrew/Cellar/fmt/10.1.1/include")
include_directories(BEFORE "/opt/homebrew/Cellar/nlohmann-json/3.11.3/include")

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU|AppleClang")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=modernize-*,-warnings-as-errors=*")
endif()

set(SOURCES sort.cpp sort_iterative.cpp introsort.cpp)

add_library(libfmt SHARED IMPORTED)
add_library(${MY_LIB_NAME} SHARED ${SOURCES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Building in Debug mode")
    target_compile_definitions(${MY_LIB_NAME} PRIVATE DEBUG_MODE)
    target_compile_options(${MY_LIB_NAME} PRIVATE -g -O0 -stdlib=libc++ -fexperimental-library -Wreturn-stack-address -pedantic -Werror)

elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Building in Release mode")
    target_compile_options(${MY_LIB_NAME} PRIVATE -O3)
endif()

find_package(fmt)
find_package(nlohmann_json)

target_link_libraries(${MY_LIB_NAME} PRIVATE fmt::fmt)
target_compile_features(${MY_LIB_NAME} PRIVATE cxx_std_20)