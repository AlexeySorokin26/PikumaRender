git clone https://github.com/Microsoft/vcpkg.git
./bootstrap-vcpkg.sh

cmake .. 


Terms:

Linear transformations
Сохраняет прямые линии прямыми не делает их кривыми и начало координат остается на месте
Масштабирование поворот не требует 4 на 4
Перенос требует матрицы 4 на 4

Гомогенные координаты — это система координат, широко используемая в компьютерной графике и геометрии, где точка в N-мерном пространстве представляется (N+1)-мерным вектором.

Матрица переноса не работает если мы будет использовать матрицу 3на3

World matrix is a combination of translation * rotation * scale matrix so all the matrices that transform the original vectors and place the overall 3D mesh in its correct place in the world/scene

