#include <iostream>

struct TInner                           // тривиальный объект: время жизни начинается когда аллоцируется в хранилище
{
    int a;
                                        // тривиальный деструктор, время жизни заканчивается при деаллокации/переиспользовании
};

class TOuter
{
public:
    const TInner& Inner;
    TOuter(const TInner& inner) :       // нетривиальный конструктор, время жизни объекта начинается после завершения инициализации
        Inner(inner) 
    {
    }
                                        // тривиальный деструктор, время жизни объекта заканчивается при деаллокации/переиспользовании
};

TInner GetInner()
{
    return {};
}

int fail() {
    TOuter outer(GetInner());           // 1. Создание временного объекта TInner (tmp_ti) вызовом GetInner()
                                        // 2. const TInner& на временный объект (tmp_ti) передается в констрктор TOuter
                                        // 3. Аллоцируется outer и начинается инициализация
                                        // 4. мембер константная ссылка Inner инициализируется константной ссылкой на временный объект tmp_ti
                                        //      пролонгация жизни временного объекта вренем жизни константной ссылки
                                        // 5. выполняется пустое тело конструктора. Начало жизни outer. Конец жизни временной константной ссылки
                                        // 6. конец выражения. разрушение мертвых временных объеков(таких нет).
    return outer.Inner.a;               // 7. копирование результата в стэк
}                                       // 8. деалокация примитивных объектов из хранилища
                                        // 9. конец жизни outer
                                        //10. конец жизни tmp_ti


int main()
{
    std::cout << fail() << std::endl;
}