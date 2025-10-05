git clone https://github.com/Microsoft/vcpkg.git
./bootstrap-vcpkg.sh
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
