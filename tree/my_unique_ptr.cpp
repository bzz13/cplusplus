#include <iostream>
#include <stdexcept>

template<typename T>
class my_unique_ptr
{
    T* data;
    bool isOwner = true;
    bool released = false;
public:
    my_unique_ptr(T* t = nullptr): data(t)
    {}

    my_unique_ptr(my_unique_ptr<T>& other): data(other.data)
    {
        other.isOwner = false;
        other.release();
    }

    ~my_unique_ptr()
    {
       release();
    }

    my_unique_ptr<T>& operator=(my_unique_ptr<T>&) = delete;

    operator bool()
    {
        return !released && data != nullptr;
    }

    T& operator*()
    {
        if(!(*this))
        {
            throw std::logic_error("ptr was released");
        }
        return *data;
    }

    T* operator->()
    {
        if(!(*this))
        {
            throw std::logic_error("ptr was released");
        }
        return data;
    }

    T* get()
    {
        return data;
    }

    void release()
    {
        if (isOwner)
        {
            delete data;
        }
        isOwner = false;
        released = true;
    }

    void move(my_unique_ptr<T>& other)
    {
        if(!(*this))
        {
            throw std::logic_error("ptr was released");
        }
        other.release();
        other.data = data;
        other.isOwner = isOwner;
        other.released = released;
        isOwner = false;
        release();
    }
};

void test1()
{
    my_unique_ptr<int> a(new int(123));
    if(a)
    {
        std::cout << *a << std::endl;
    }
    a.release();
    if(a)
    {
        std::cout << *a << std::endl;
    }
    try
    {
        std::cout << *a << std::endl;
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void test2()
{
    struct c
    {
        int value;
        c(int v): value(v) {}
    };
    my_unique_ptr<c> a(new c(123));
    if(a)
    {
        std::cout << a->value << std::endl;
    }
    a.release();
    if(a)
    {
        std::cout << a->value << std::endl;
    }
    try
    {
        std::cout << a->value << std::endl;
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void test3()
{
    my_unique_ptr<int> a(new int(321));
    if(a)
    {
        std::cout << "a point to " << *a << std::endl;
    }
    {
        my_unique_ptr<int> b(a);
        if(b)
        {
            std::cout << "b point to " << *b << std::endl;
        }
        *b = 123;

        try
        {
            std::cout << *a << std::endl;
        }
        catch(std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }

        {
            if(b)
            {
                std::cout << "b point to " << *b << std::endl;
            }
            my_unique_ptr<int> c;
            b.move(c);

            if(b)
            {
                std::cout << "b point to " << *b << std::endl;
            }
            if(c)
            {
                std::cout << "c point to " << *c << std::endl;
            }
        }
    }

    try
    {
        std::cout << *a << std::endl;
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void test4()
{
    struct d
    {
        ~d()
        {
            std::cout << "destructor" << std::endl;
        }
    };

    {
        my_unique_ptr<d> a(new d);
        {
            my_unique_ptr<d> b(a);
            {
                my_unique_ptr<d> c;
                b.move(c);
                // b.move(a);
                // c.move(b);
                c.move(a);
            }
            std::cout << "dest c" << std::endl;
        }
        std::cout << "dest b" << std::endl;
    }
    std::cout << "dest a" << std::endl;
}

int main()
{
    test1();
    test2();
    test3();
    test4();
    return 0;
}