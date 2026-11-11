cmake_minimum_required(VERSION 3.20)

project(calculator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(
	-g
	-fpermissive
	-Wall
	-Wextra
	-Werror
	-Wpedantic
)

include(FetchContent)

FetchContent_Declare(
    math
    GIT_REPOSITORY https://github.com/DRUsmanov/math.git
    GIT_TAG main
)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.17.0
)

set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS   OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(math)
FetchContent_MakeAvailable(nlohmann_json)
FetchContent_MakeAvailable(spdlog)

file(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS
src/calculator/*cpp
src/logger/*cpp
)

add_library(source STATIC ${SOURCE})
target_include_directories(source PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(source PRIVATE math::math nlohmann_json::nlohmann_json spdlog::spdlog)

add_executable(calculator src/main.cpp)
target_link_libraries(calculator PRIVATE source)

if(USE_CLANG_FORMAT_CHECK)
	find_program(CLANG_FORMAT_PATH NAMES clang-format-21 REQUIRED)
	message(STATUS "Found clang format: ${CLANG_FORMAT_PATH}")
	file(GLOB_RECURSE CLANG_FORMAT_SRC CONFIGURE_DEPENDS
	src/*.h
	src/*.cpp)

    add_custom_target(clang-format-check
            COMMAND
                run-clang-format.py --clang-format-executable=${CLANG_FORMAT_PATH} --style=file ${CLANG_FORMAT_SRC}
            WORKING_DIRECTORY
                ${CMAKE_SOURCE_DIR}
            COMMENT
                "Checking format ${CLANG_FORMAT_DEMO} with ${CLANG_FORMAT_PATH} ..."
    )

    add_dependencies(source clang-format-check)
endif()

if(USE_CLANG_FORMAT_FIX)
    find_program(CLANG_FORMAT_PATH NAMES clang-format-21 REQUIRED)
    message(STATUS "Found clang-format: ${CLANG_FORMAT_PATH}")

    file(GLOB_RECURSE CLANG_FORMAT_SRC CONFIGURE_DEPENDS
        src/*.h
        src/*.cpp
        src/*.hpp
    )

    add_custom_target(clang-format-fix
        COMMAND
            ${CLANG_FORMAT_PATH} -style=file -i ${CLANG_FORMAT_SRC}
        WORKING_DIRECTORY
            ${CMAKE_SOURCE_DIR}
        COMMENT
            "Formatting ${CLANG_FORMAT_DEMO} with ${CLANG_FORMAT_PATH} ..."
    )

    add_dependencies(source clang-format-fix)
endif()

if(USE_CLANG_TIDY)
    find_program(CLANG_TIDY_PATH NAMES clang-tidy-21 REQUIRED)
    message(STATUS "Found clang-tidy: ${CLANG_TIDY_PATH}")
    set(CLANG_TIDY_COMMAND ${CLANG_TIDY_PATH} "-config-file=${CMAKE_SOURCE_DIR}/.clang-tidy")
    set_target_properties(calculator  PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()


#Tests

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  GIT_TAG main
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB_RECURSE TESTS_SOURCE CONFIGURE_DEPENDS tests/*cpp)

add_executable(tests ${TESTS_SOURCE})
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(tests GTest::gtest_main source)

include(GoogleTest)

gtest_discover_tests(tests)
