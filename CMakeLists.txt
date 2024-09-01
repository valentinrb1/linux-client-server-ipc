cmake_minimum_required(VERSION 3.22)
project(client_server)

file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/files)

set(CMAKE_C_COMPILER gcc)

set(SOURCES_C src/clients.c)
set(HEADERS_C inc/clients.h)

set(SOURCES_S src/server.c)
set(HEADERS_S inc/server.h)

add_executable(clients ${SOURCES_C} ${HEADERS_C})
add_executable(server ${SOURCES_S} ${HEADERS_S})

target_compile_options(clients PRIVATE -Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11 -g)
target_compile_options(server PRIVATE -Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11 -g)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${PROJECT_SOURCE_DIR}/bin")