cmake_minimum_required(VERSION 3.15)
project(MySDLApp)

set(CMAKE_TOOLCHAIN_FILE "C:/Users/PC/Desktop/Pikuma/vcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(SDL2 REQUIRED)
add_executable(sdl-app src/main.cpp)
target_link_libraries(sdl-app PRIVATE SDL2::SDL2 SDL2::SDL2main)