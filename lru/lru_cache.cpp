#include <iostream>
#include <unordered_map>
#include <vector>
#include <time.h>

using namespace std;

template<typename TKey, typename TValue>
class LRUCache 
{

    struct LRURecord
    {
        TValue value;
        long long last_update;
    };

    int m_ttl;
    size_t m_max_size;
    unordered_map<TKey, LRURecord> m_map;

    long long now()
    {
        return (long long) time(0);
    }
    void remove_oldest()
    {
        long long min_last_update;
        TKey min_key;
        for(auto p: m_map)
        {
            if (p.second.last_update <= min_last_update)
            {
                min_last_update = p.second.last_update;
                min_key = p.first;
            }
        }
        m_map.erase(min_key);
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
        m_map[key] = LRURecord{value, now()};
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
            if (find->second.last_update < now() - m_ttl)
            {
                m_map.erase(find);
                return false;
            }
            else
            {
                value = find->second.value;
                m_map[key].last_update = now();
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
    return 0;
}