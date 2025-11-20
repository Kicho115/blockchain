cmake_minimum_required(VERSION 3.21)

set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic" CACHE STRING "")
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

project(blockchain CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)

include_directories(src)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_executable(blockchain ${SOURCES})
target_link_libraries(blockchain PRIVATE OpenSSL::SSL OpenSSL::Crypto)