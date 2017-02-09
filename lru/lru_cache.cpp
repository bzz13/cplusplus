#include <iostream>
#include <unordered_map>
#include <vector>
#include <list>
#include <time.h>
#include <chrono>
#include <thread>

using namespace std;

template<typename TKey, typename TValue>
class LRUCache
{
    typedef long long second;
    typedef typename std::list<TKey>::iterator TKeyPos;

    struct LRURecord
    {
        TValue value;
        second last_update;
        TKeyPos position;
    };

    int m_ttl;
    size_t m_max_size;
    list<TKey> m_list;
    unordered_map<TKey, LRURecord> m_map;

    second now()
    {
        return (second) time(0);
    }
    void remove_oldest()
    {
        auto begin = m_list.begin();
        if(begin != m_list.end())
        {
            m_map.erase(*begin);
            m_list.erase(begin);
        }
    }
    void set(const TKey& key, const TValue& value)
    {
        m_list.push_back(key);
        m_map[key] = LRURecord{value, now(), -- (m_list.end())};
    }
public:

    LRUCache(int ttl, size_t max_size): m_ttl(ttl), m_max_size(max_size)
    {}


    void put(const TKey& key, const TValue& value)
    {
        auto find = m_map.find(key);
        if (find != m_map.end())
        {
            m_list.erase(find->second.position);
        }
        else
        {
            if (m_map.size() >= m_max_size)
            {
                remove_oldest();
            }
        }
        set(key, value);
    }
    bool get(const TKey& key, TValue& value)
    {
        auto find = m_map.find(key);
        if (find == m_map.end())
        {
            return false;
        }
        else
        {
            m_list.erase(find->second.position);
            if (find->second.last_update < now() - m_ttl)
            {
                m_map.erase(find);
                return false;
            }
            else
            {
                value = find->second.value;
                set(key, value);
                return true;
            }
        }
    }

    void printState()
    {
        cout << "list: ";
        for(auto l: m_list)
        {
            cout << l << " ";
        }
        cout << endl
             << "map: " << endl;
        for(auto p: m_map)
        {
            cout << p.first << " {" << p.second.value << ", " << p.second.last_update << ", " << *(p.second.position) << "}" << endl;
        }
    }
};


void print(LRUCache<int, int>& cache)
{
    int result;
    cout <<"\"";
    if (cache.get(1, result))
    {
        cout << result << " ";
    }
    if (cache.get(2, result))
    {
        cout << result << " ";
    }
    if (cache.get(3, result))
    {
        cout << result << " ";
    }
    cout << "\"" << endl;
    // cache.printState();
    // cout << endl;
}

void T1()
{
    LRUCache<int, int> cache(100, 2);
    cache.put(1, 1);
    // cache.printState();
    cache.put(2, 2);
    print(cache); // 1 2
    // cache.printState();
    cache.put(3, 3); // drop "1"
    print(cache); // 2 3
    cache.put(1, 1); // drop "2"
    print(cache); // 1 3
    cout << endl;
}

void T2()
{
    LRUCache<int, int> cache(100, 2);
    cache.put(1, 1); // 
    cache.put(1, 2); // update by key 1 -> 2
    cache.put(1, 20); // update by key 2 -> 20
    print(cache); // 20
    cache.put(2, 3);
    print(cache); // 20 3
    cache.put(3, 4); // drop 1
    print(cache); // 3 4
    cout << endl;
}

void T3()
{
    using namespace std::chrono_literals;
    LRUCache<int, int> cache(5, 2);
    cache.put(1, 1);
    this_thread::sleep_for(3s);
    cache.put(2, 2);
    this_thread::sleep_for(3s);
    print(cache); // 2
    this_thread::sleep_for(7s);
    print(cache); // empty
    cout << endl;
}

int main()
{
    T1();
    T2();
    T3();
    return 0;
}