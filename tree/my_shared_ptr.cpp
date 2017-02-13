#include <iostream>
#include <stdexcept>


template<typename T>
class my_shared_ptr
{
    struct ref_counter
    {
        size_t count;
        ref_counter(): count(1) { }
        void increase() { ++count; }
        size_t decrease() { return (--count); }
    };

    ref_counter* counter;
    T* data;
    bool released = false;

public:
    my_shared_ptr(T* t = nullptr): data(t), counter(new ref_counter())
    {
    }

    my_shared_ptr(const my_shared_ptr<T>& other): data(other.data), counter(other.counter)
    {
        counter->increase();
    }

    ~my_shared_ptr()
    {
        release();
    }

    void release()
    {
        released = true;
        if (counter->decrease() == 0)
        {
            delete data;
            delete counter;
        }
    }

    my_shared_ptr<T>& operator=(const my_shared_ptr<T>& other)
    {
        if (this != &other)
        {
            release();
            released = false;
            data = other.data;
            counter = other.counter;
            counter->increase();
        }
        return *this;
    }

    operator bool()
    {
        return !released && bool(counter) && counter->count > 0 && bool(data);
    }

    T& operator*()
    {
        if(!(*this))
            throw std::logic_error("ptr was released");
        return *data;
    }

    T* operator->()
    {
        if(!(*this))
            throw std::logic_error("ptr was released");
        return data;
    }

};

void test1()
{
    my_shared_ptr<int> mptr(new int(152));
    if(mptr)
    {
        std::cout << *mptr << std::endl;
        *mptr = 432;
        std::cout << *mptr << std::endl;
    }
}

void test2()
{
    my_shared_ptr<int> a(new int(123));
    my_shared_ptr<int> b(a);

    a.release();
    try
    {
        std::cout << *a << std::endl;
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    if (b)
    {
        std::cout << *b << std::endl;
    }
    else
    {
        std::cout << "b released" << std::endl;
    }
}

void test3()
{
    my_shared_ptr<int> a(new int(321));
    my_shared_ptr<int> b;

    if(b)
    {
        std::cout << *b << std::endl;
    }
    else
    {
        std::cout << "b not init" << std::endl;
    }

    b = a;

    if(b)
    {
        std::cout << *b << std::endl;
    }
    else
    {
        std::cout << "b not init" << std::endl;
    }
}

void test4()
{
    struct c
    {
        int value;
        c(int val): value(val) { }
        ~c(){ std::cout << "call destructor" << std::endl; }
    };

    {
        my_shared_ptr<c> a(new c(555));
        {
            my_shared_ptr<c> b(a);
            if(b)
            {
                std::cout << b->value << std::endl;
                (*b).value = 777;
            }
        }
        if (a)
        {
            std::cout << a->value << std::endl;
        }
    }
}

int main()
{
    test1();
    test2();
    test3();
    test4();
    return 0;
}