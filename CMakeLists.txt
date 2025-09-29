cmake_minimum_required(VERSION 3.15)
project(MySDLApp)

set(CMAKE_TOOLCHAIN_FILE "C:/Users/PC/Desktop/Pikuma/vcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(SDL2 REQUIRED)
add_subdirectory(src)