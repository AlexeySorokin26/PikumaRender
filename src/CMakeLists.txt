set(headers display.h vector.h camera.h)
set(srcs main.cpp display.cpp vector.cpp camera.cpp)

add_executable(sdl-app ${headers} ${srcs})
target_link_libraries(sdl-app PRIVATE SDL2::SDL2 SDL2::SDL2main)