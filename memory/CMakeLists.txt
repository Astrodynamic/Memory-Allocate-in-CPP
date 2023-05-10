cmake_minimum_required(VERSION 3.5)

project(Memory VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/memory.h
)

set(SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/memory.cc
)

add_library(
  ${PROJECT_NAME}
  STATIC
  ${HEADERS}
  ${SOURCES}
)

target_compile_options(
  ${PROJECT_NAME}
  PRIVATE
  -Wall
  -Werror
  -Wextra
  -Wpedantic
)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")

set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
