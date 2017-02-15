./a | ./b

./a - 1G/s stdout
./b - 1K/s stdin


------------------
new malloc

//////

vector<int> v; 

data = new(sizeof(T)*size)


v.push_back(1);

//////

class ref_counter
{
    size_t count;
public:
    ref_counter(): count(1) { }

    void add()
    {
        count++;
    }
    
    size_t release()
    {
        return --count;
    }
};

template<typename T>
class my_shared_ptr
{
    ref_counter* counter;
    T* data;
public:
    explicite my_shared_ptr(T* t = nullptr): data(t), counter(new ref_counter())
    {}
    
    my_shared_ptr(const my_shared_ptr<T>& other): data(other.data), counter(other.counter)
    {
        counter->add();        
    }
    
    ~my_shared_ptr()
    {
        release();
    }
    
    void release()
    {
        if (counter->release() == 0)
        {
            delete data;
            delete counter;
            data = nullptr;
            counter = nullptr;
        }
    }
    
    my_shared_ptr<T>& operatror=(const my_shared_ptr<T>& other)
    {
        if (this != &other)
        {
            release();
            date = other.data;
            counter = other.data;
            counter->add();
        }
        return *this;
    }
    
    T& operator*() const
    {
        return *data;
    }
    
    T* operator->() const
    {
        return data;
    }
};



int main()
{
    int* p = new int(123);
    p->release();
    my_shared_ptr<int> ptr = tmp;
}



