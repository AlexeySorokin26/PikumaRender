cmake_minimum_required(VERSION 3.15)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

project(MySDLApp LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(APPLE)
    add_compile_options(-stdlib=libc++)
    link_libraries(-stdlib=libc++)
endif()

find_package(SDL2 REQUIRED)

if(SDL2_FOUND)
    message(STATUS "SDL2 found: ${SDL2_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "SDL2 not found!")
endif()

add_subdirectory(src)