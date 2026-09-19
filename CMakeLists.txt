cmake_minimum_required(VERSION 3.20)

project(calculator)

include(FetchContent)

FetchContent_Declare(
    math
    GIT_REPOSITORY https://github.com/DRUsmanov/math.git
    GIT_TAG main
)

FetchContent_MakeAvailable(math)

add_executable(calculator calculator.cpp)

target_link_libraries(calculator PRIVATE math::math)
