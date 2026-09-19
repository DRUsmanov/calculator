cmake_minimum_required(VERSION 3.20)

project(calculator)

include(FetchContent)

FetchContent_Declare(
    math
    GIT_REPOSITORY https://github.com/DRUsmanov/math.git
    GIT_TAG main
)

FetchContent_MakeAvailable(math)

add_executable(calculator src/calculator.cpp)

set_target_properties(calculator
	PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

target_compile_options(calculator
	PRIVATE
	-g
	-fpermissive
	-Wall
	-Wextra
	-Werror
	-Wpedantic
)

if(USE_CLANG_FORMAT_CHECK)
	find_program(CLANG_FORMAT_PATH NAMES clang-format-21 REQUIRED)
	message(STATUS "Found clang format: ${CLANG_FORMAT_PATH}")
	file(GLOB_RECURSE CLANG_FORMAT_SRC CONFIGURE_DEPENDS
	src/*.h
	src/*.cpp
)

add_custom_target(clang-format-fix
        COMMAND
            ${CLANG_FORMAT_PATH} -style=file -i ${CLANG_FORMAT_SRC}
        WORKING_DIRECTORY
            ${CMAKE_SOURCE_DIR}
        COMMENT
            "Formatting ${CLANG_FORMAT_DEMO} with ${CLANG_FORMAT_PATH} ..."
)

add_custom_target(clang-format
        COMMAND
            run-clang-format.py --clang-format-executable=${CLANG_FORMAT_PATH} --style=file ${CLANG_FORMAT_SRC}
        WORKING_DIRECTORY
            ${CMAKE_SOURCE_DIR}
        COMMENT
            "Checking format ${CLANG_FORMAT_DEMO} with ${CLANG_FORMAT_PATH} ..."
)

    add_dependencies(calculator clang-format)
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

    add_dependencies(calculator clang-format-fix)
endif()

if(USE_CLANG_TIDY)
    find_program(CLANG_TIDY_PATH NAMES clang-tidy-21 REQUIRED)
    message(STATUS "Found clang-tidy: ${CLANG_TIDY_PATH}")
    set(CLANG_TIDY_COMMAND ${CLANG_TIDY_PATH} "-config-file=${CMAKE_SOURCE_DIR}/.clang-tidy")
    set_target_properties(calculator  PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()


target_link_libraries(calculator PRIVATE math::math)
