#include <iostream>
#include <string>
#include <exception>

using namespace std;

template<typename T>
class my_unique_ptr
{
	T* ptr;
	bool isOwner;

	void release() 
	{
		if (isOwner && ptr != nullptr)
			delete ptr;
	}
	void move(my_unique_ptr<T>& other)
	{
		isOwner = other.isOwner;
		other.isOwner = false;
		ptr = other.ptr;
	}

public:
	my_unique_ptr(T* p = nullptr): ptr(p), isOwner(false) 
	{
		isOwner = p != nullptr;
	}

	my_unique_ptr(my_unique_ptr<T>& other): ptr(nullptr), isOwner(false) 
	{
		if (&other != this)
		{
			release();
			move(other);
		}
	}

	my_unique_ptr<T>& operator=(my_unique_ptr<T>& other)
	{
		if (&other != this)
		{
			release();
			move(other);
		}
		return *this;
	}

	T* operator->() const 
	{
		return ptr;
	}

	T& operator*() const
	{
		if (ptr != nullptr)
			return *ptr;
		throw exception();
	}

	operator bool() const
	{
		return ptr != nullptr && isOwner;
	}

	~my_unique_ptr()
	{
		release();
	};

	friend ostream& operator<<(ostream& os, const my_unique_ptr<T>& my) 
	{
		return (os << "state: { ptr= " << my.ptr << ", isOwner= " << my.isOwner << " }" << endl );
	}
};

struct Foo {
	Foo() { cout << "Foo::Foo" << endl; }
	~Foo() { cout << "Foo::~Foo" << endl; }
	void bar() { cout << "Foo::bar" << endl; }
};
 
void f(const Foo &foo)
{
	cout << "f(const Foo&)" << endl;
}

int main()
{
	{
		my_unique_ptr<Foo> p1(new Foo);	// p1 владеет Foo
		if (p1)
		{
			p1->bar();
		}
		{
			my_unique_ptr<Foo> p2(p1);	// теперь p2 владеет Foo
			cout << endl << "p1= " << p1 << "p2= "<< p2 << endl;
			f(*p2);
			p1 = p2;					// владение возвращено p1
			cout << endl << "p1= " << p1 << "p2= "<< p2 << endl;
			cout << "destroying p2..." << p2;
			my_unique_ptr<Foo> p3;		// теперь p3 не владеет ничем
			p3 = p1;					// теперь p3 владение Foo
			p1 = p3;					// владение возвращено p1
			cout << "destroying p3..." << p3;
		}

		if (p1)
		{
			p1->bar();
		}

		cout << "destroying p1..." << p1;
	}
	return 0;
}