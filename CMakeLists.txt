cmake_minimum_required(VERSION 3.15)

project(MemoryLogic VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
  ${CMAKE_SOURCE_DIR}/memory
  ${CMAKE_SOURCE_DIR}/interface
)

set(HEADERS
  ${CMAKE_SOURCE_DIR}/interface/ainterface.h
  ${CMAKE_SOURCE_DIR}/interface/interface.h
)

set(SOURCES
  ${CMAKE_SOURCE_DIR}/interface/ainterface.cc
  ${CMAKE_SOURCE_DIR}/interface/interface.cc
)

add_executable(
  ${PROJECT_NAME}
  ${HEADERS}
  ${SOURCES}
  ${CMAKE_SOURCE_DIR}/main.cc
)

add_subdirectory(${CMAKE_SOURCE_DIR}/memory)

target_compile_options(
  ${PROJECT_NAME}
  PRIVATE
  -Wall
  -Werror
  -Wextra
  -Wpedantic
)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
  Memory
)

# Add cppcheck
find_program(CPPCHECK cppcheck)

file(GLOB_RECURSE ALL_SOURCE_FILES *.cc *.h)
list(FILTER ALL_SOURCE_FILES EXCLUDE REGEX ".*build.*")

if(CPPCHECK)
  message(STATUS "cppcheck found: ${CPPCHECK}")
  list(
    APPEND CPPCHECK_ARGS
    "--enable=all"
    "--inconclusive"
    "--quiet"
    "--language=c++"
    "--std=c++17"
    "--suppress=unusedFunction"
    "--suppress=missingInclude"
    "--suppress=functionStatic"
    "--suppress=invalidScanfArgType_int"
    "--verbose"
  )
  add_custom_target(
    cppcheck
    COMMAND ${CPPCHECK} ${CPPCHECK_ARGS} ${ALL_SOURCE_FILES}
  )
else()
  message(STATUS "cppcheck not found")
endif()

# Add clang-format
find_program(CLANG_FORMAT clang-format)

if(CLANG_FORMAT)
  message(STATUS "clang-format found: ${CLANG_FORMAT}")
  list(
    APPEND CLANG_ARGS
    "-n"
    "-style=Google"
    "--verbose"
  )
  add_custom_target(
    clang-format
    COMMAND ${CLANG_FORMAT} ${CLANG_ARGS} ${ALL_SOURCE_FILES}
  )
else()
  message(STATUS "clang-format not found")
endif()