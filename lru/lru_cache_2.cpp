#include <iostream>
#include <unordered_map>
#include <memory>

using namespace std;

template<typename TKey, typename TValue>
class LRUCache2
{
    struct ListNode
    {
        ListNode(const TKey& k, shared_ptr<ListNode> p = nullptr, shared_ptr<ListNode> n = nullptr)
            : key(k), prev(p), next(n)
        {}

        TKey key;
        shared_ptr<ListNode> prev;
        shared_ptr<ListNode> next;
    };

    struct List
    {
        shared_ptr<ListNode> head;
        shared_ptr<ListNode> tail;

        shared_ptr<ListNode> put_back(const TKey& key)
        {
            if (!tail)
            {
                head = make_shared<ListNode>(key);
                tail = head;
            }
            else
            {
                tail->next = make_shared<ListNode>(key, tail);
                tail = tail->next;
            }
            return tail;
        }
        void put_back(shared_ptr<ListNode>& node)
        {
            if (!tail)
            {
                head = node;
                tail = node;
                node->prev = nullptr;
                node->next = nullptr;
            }
            else
            {
                tail->next = node;
                node->prev = tail;
                node->next = nullptr;
                tail = tail->next;
            }
        }
        void cut(shared_ptr<ListNode>& node)
        {
            if (!node)
            {
                return;
            }
            if (node == head)
            {
                if(head == tail)
                {
                    head = nullptr;
                    tail = nullptr;
                }
                else
                {
                    head = head->next;
                    head->prev = nullptr;
                }
            }
            else
            {
                if (node == tail)
                {
                    tail = tail->prev;
                    tail->next = nullptr;
                }
                else
                {
                    node->prev->next = node->next;
                    node->next->prev = node->prev;
                }
            }
            node->prev = nullptr;
            node->next = nullptr;
        }

        void move_back(shared_ptr<ListNode>& node)
        {
            cut(node);
            put_back(node);
        }
    };

    struct LRUCache2Record
    {
        TValue value;
        shared_ptr<ListNode> node;
    };

    size_t max_size;
    unordered_map<TKey, LRUCache2Record> map;
    List list;
public:
    LRUCache2(size_t msize): max_size(msize)
    {}

    void put(const TKey& key, const TValue& value)
    {
        auto find = map.find(key);
        if(find != map.end())
        {
            find->second.value = value;
            list.move_back(find->second.node);
        }
        else
        {
            if(map.size() >= max_size)
            {
                auto head = list.head;
                map.erase(head->key);
                list.cut(head);
            }
            map[key] = LRUCache2Record {value, list.put_back(key)};
        }
    }
    bool get(const TKey& key, TValue& value)
    {
        auto find = map.find(key);
        if(find == map.end())
        {
            return false;
        }
        else
        {
            value = find->second.value;
            list.move_back(find->second.node);
            return true;
        }
    }

    void printState()
    {
        cout << "list: " << endl;
        auto h = list.head;
        while(h)
        {
            cout << "{" << h->prev << " <- " << h->key << " -> " << h->next << "}" << endl;
            h = h->next;
        }
        cout << "map: " << endl;
        for(auto p: map)
        {
            cout << p.first << " {" << p.second.value << ", " << &p.second.node << "}" << endl;
        }
    }
};

void print(LRUCache2<int,int>& cache)
{
    int result;
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
    if (cache.get(4, result))
    {
        cout << result << " ";
    }
    cout << endl;
}

void T1()
{
    LRUCache2<int,int>cache(2);
    print(cache); // ""
    cache.put(1,1);
    print(cache); // "1 "
    cache.put(2,2);
    print(cache); // "1 2 "
    cache.put(3,3);
    print(cache); // "2 3 "
    cache.put(1,1);
    print(cache); // "1 3 "
}

void T2()
{
    LRUCache2<int,int>cache(2);
    print(cache); // ""
    cache.put(1, 1);
    print(cache); // "1 "
    cache.put(1, 2);
    print(cache); // "2 "
    cache.put(1, 20);
    print(cache); // 20
    cache.put(2, 3);
    print(cache); // 20 3
    cache.put(2, 30);
    print(cache); // 20 30
    cache.put(3, 4);
    print(cache); // 30 4
    cout << endl;
}

int main()
{
    T1();
    T2();
    return 0;
}