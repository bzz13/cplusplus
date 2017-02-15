template<typename T>
struc node
{
    T data;
    shared_ptr<node> next;
    node(const T& t): data(t)
    {}
};

template<typename T>
struct list
{
    shared_ptr<node<T>> head;
    size_t length() const
    {
        size_t result = 0;
        auto tmp = head;
        while(tmp)
        {
            result++;
            tmp = tmp->next;
        }
        return result;
    }
    
    shared_ptr<node<T>> findIntersection(cosnt list<T>& a, const list<T>& b)
    {
        auto lengthA = a.length();
        auto lengthB = b.length();
        auto diff = lengthA > lengthB ? lengthA - lengthB : lengthB - lengthA;
        
        node<T> tmp = lengthA > lengthB ? a.head : b.head;
        for(int i = 0; i < diff; ++i)
        {
            tmp = tmp->next;
        }
        node<T> other = lengthA <= lengthB ? a.head : b.head;
        
        while(tmp != other)
        {
            tmp = tmp->next;
            other = other->next;
        }
        return tmp;
    }
};

1 -> 2 -> 3 -> 4 -> 5 - 6 - 7  - 8 - 9 - 10
                                 |
                           12 - 13 