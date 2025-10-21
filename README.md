git clone https://github.com/Microsoft/vcpkg.git
./bootstrap-vcpkg.sh

cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake


Terms:

Linear transformations
Сохраняет прямые линии прямыми не делает их кривыми и начало координат остается на месте
Масштабирование поворот не требует 4 на 4
Перенос требует матрицы 4 на 4

Гомогенные координаты — это система координат, широко используемая в компьютерной графике и геометрии, где точка в N-мерном пространстве представляется (N+1)-мерным вектором.
