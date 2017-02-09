#include <iostream>
#include <unordered_map>
#include <vector>
#include <list>
#include <time.h>

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
        if (m_map.size() >= m_max_size)
        {
            remove_oldest();
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
};

int main()
{
    LRUCache<int, int> cache(100, 2);
    cache.put(1, 1);
    cache.put(2, 2);
    cache.put(3, 3);

    int result;
    if (cache.get(1, result))
    {
        cout << result << endl;
    }
    if (cache.get(2, result))
    {
        cout << result << endl;
    }
    if (cache.get(3, result))
    {
        cout << result << endl;
    }

    cache.put(1, 1);
    if (cache.get(1, result))
    {
        cout << result << endl;
    }
    if (cache.get(2, result))
    {
        cout << result << endl;
    }
    if (cache.get(3, result))
    {
        cout << result << endl;
    }
    return 0;
}