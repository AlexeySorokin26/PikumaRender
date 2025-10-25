git clone https://github.com/Microsoft/vcpkg.git
./bootstrap-vcpkg.sh

cmake .. 


Terms:

Linear transformations
Сохраняет прямые линии прямыми не делает их кривыми и начало координат остается на месте
Масштабирование поворот не требует 4 на 4
Перенос требует матрицы 4 на 4

1. Определение линейного преобразования
Преобразование T является линейным если:

T(u + v) = T(u) + T(v) - аддитивность

T(αu) = αT(u) - однородность
Пусть перенос на вектор t: T(v) = v + t
Проверяем аддитивность:
T(u + v) = (u + v) + t
T(u) + T(v) = (u + t) + (v + t) = u + v + 2t
❌ T(u + v) ≠ T(u) + T(v) (t ≠ 2t)
Проверяем однородность:
T(αu) = αu + t  
αT(u) = α(u + t) = αu + αt
❌ T(αu) ≠ αT(u) (t ≠ αt)

Гомогенные координаты — это система координат, широко используемая в компьютерной графике и геометрии, где точка в N-мерном пространстве представляется (N+1)-мерным вектором.

Матрица переноса не работает если мы будет использовать матрицу 3на3
Translation is not a linear transformation
because the change the origin of our object (we just move it)

World matrix is a combination of translation * rotation * scale matrix so all the matrices that transform the original vectors and place the overall 3D mesh in its correct place in the world/scene

Projection Matrix responsible for
1. Aspect ratio: adjust x and y values based on the screen width & height values
 a = h/w
2. Field of view: -//-
 tanQ/2 = opposite / adjacent
 f = 1/tanQ/2
3. Normalization: adjust x,y,z to be in -1 to +1 NDC
 zFar zNear
 l = zFar / (zFar - zNear) - zFar/(zFar - zNear) *zNear

 Выводится за два этапа
 делаем проекцию считаем x y z проекционные затем переходит в ndc через подобные отрезки	