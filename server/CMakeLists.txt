cmake_minimum_required(VERSION 3.1)
project(server)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCAMERA REQUIRED libcamera)

add_executable(server
        main.cpp
        constants.h
        server.cpp
        server.h
)

target_include_directories(server PRIVATE
        ${LIBCAMERA_INCLUDE_DIRS}
)

target_link_directories(server PRIVATE
        ${LIBCAMERA_LIBRARY_DIRS}
)

target_link_libraries(server PRIVATE
        ${LIBCAMERA_LIBRARIES}
)

target_compile_options(server PRIVATE
        ${LIBCAMERA_CFLAGS_OTHER}
)