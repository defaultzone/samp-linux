cmake_minimum_required(VERSION 3.25)
project(gtksamp C)

set(CMAKE_C_STANDARD 23)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})
add_definitions(${GTK4_CFLAGS_OTHER})

add_executable(
        samp-linux # Project.
        src/main.c
        src/actions.c
        src/actions.h
        src/main.h
        src/config.c
        src/config.h
        src/window.c
        src/window.h
        src/path.c
        src/path.h
        src/string.h
        src/string.c)
target_link_libraries(samp-linux ${GTK4_LIBRARIES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/cmake-build)

#
# Installation
#

install(TARGETS samp-linux DESTINATION /usr/bin/)   # Making samp-linux executable.
install(DIRECTORY usr/ DESTINATION /usr)            # Moving usr/ to /usr.