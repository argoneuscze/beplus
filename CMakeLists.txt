cmake_minimum_required (VERSION 3.1)
project(beplus)

set(CMAKE_CXX_STANDARD 14)

add_executable(beplus "")

include(src/CMakeLists.txt)
include(src/simple_interpreter/CMakeLists.txt)
include(src/parser/CMakeLists.txt)
include(src/parser/ast/CMakeLists.txt)
