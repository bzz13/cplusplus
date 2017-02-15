cache{0,1,2,3}.yandex.ru

API:
    put(key k, image i)
    bool get(key k, image& i)


1. Полный перебор
2. Таблица с индексом на клиенте
3.
int indexOfRemote(url, count)
{
    //использовать список
    return hash1(url) % count;
}

4. При выпадении машинки ходим на следующую по списку.

5. cache4.yandex.ru

5.1. Схема с несколькими мастер-машинами
репликация?
market-cache{0,1,2}.yandex.ru
0 - dead
клиент -> 1
lead - 2

http://la-la-la.com/12.png
http://bu-bu-bu.com/32.png