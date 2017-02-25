#include <iostream>

struct TInner
{
    int a;
    ~TInner()
    {
        std::cout << "~TInner" << std::endl;
    }
};

class TOuter
{
public:
    const TInner& Inner;
    TOuter(TInner&& inner) :
        Inner(inner) 
    {
    }
    ~TOuter()
    {
        std::cout << "~TOuter" << std::endl;
    }
};

TInner GetInner()
{
    return {3};
}


int ff(int c)
{
    for (int i = 0 ; i < c; i ++)
    {
        std::cout << (i*i)  << " ";
    }
    std::cout<<std::endl;
}

int fail() {
    std::cout << "expression 1 " << std::endl;
    TOuter outer(GetInner());           // 1. Создание временного объекта TInner (tmp_ti) вызовом GetInner()
                                        // 2. временная const TInner& на временный объект (tmp_ti) передается в констрктор TOuter
                                        // 3. Аллоцируется outer и начинается инициализация
                                        // 4. мембер константная ссылка Inner инициализируется константной ссылкой на временный объект tmp_ti
                                        //      вторая константная ссылка не пролонгирует жизнь временного объекта
                                        // 5. выполняется пустое тело конструктора. Начало жизни outer. 
                                        // 6. конец выражения. конец жизни временной константной ссылки и временного объекат tmp_ti. деструктор tmp_ti

    std::cout << "expression 2 " << std::endl;
    return outer.Inner.a;               // 7. обращение к разрушенному объекту, копирование мусора в стэк
}                                       // 8. деалокация объектов из хранилища
                                        // 9. конец жизни outer


int main()
{
    std::cout << fail()<< std::endl;
}