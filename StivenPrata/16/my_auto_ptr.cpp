#include <iostream>
#include <string>
using namespace std;

template<typename T>
class my_auto_ptr
{
	T* pt;

	void release()
	{
		if (pt != nullptr)
		{
			cout << "\trelease: " << pt << endl;
			delete pt;
			pt = nullptr;
		}
	}

public:
	my_auto_ptr(T* t = nullptr) : pt(t)
	{
		cout << "default ctor: " << t << endl;
	};

	my_auto_ptr(const my_auto_ptr<T>& p): pt(nullptr)
	{
		cout << "copy ctor: " << p.pt << " to " << pt << endl;
		release();
		pt = new T(*(p.pt));
		cout << "\tnew loc: " << pt << endl;
	};
	~my_auto_ptr()
	{
		cout << "dtor" << endl;
		release();
	};

	my_auto_ptr<T>& operator=(const my_auto_ptr<T>& p)
	{
		cout << "operator=(<T>&) " << p.pt << " to " << pt << endl;
		release();
		pt = new T(*(p.pt));
		cout << "\tnew loc: " << pt << endl;
		return *this;
	}
	my_auto_ptr<T>& operator=(T* p)
	{
		cout << "operator=(T*) " << p << " to " << pt << endl;
		release();
		pt = p;
		cout << "\tnew loc: " << pt << endl;
		return *this;
	}

	T& operator*()
	{
		cout << "operator*" << endl;
		return *pt;
	}

	const T* const operator->()
	{
		cout << "operator->" << endl;
		return pt;
	}
};

class tc
{
	int val;
public:
	tc(int v): val(v) { };
	int Val() const { return val; }
};

int main()
{
	{
		cout << endl;
		my_auto_ptr<int> p;
	}
	{
		cout << endl;
		my_auto_ptr<int> p(new int(234));
		cout << *p << endl;
		*p = 543;
		cout << *p << endl;
	}
	{
		cout << endl;
		my_auto_ptr<int> p(new int(2342));
		my_auto_ptr<int> p1(p);
	}
	{
		cout << endl;
		my_auto_ptr<int> p(new int(2342));
		my_auto_ptr<int> p1 = new int(654);
		cout << "values: " << *p << " " << *p1 << endl;
		p = p1;
		cout << "values: " << *p << " " << *p1 << endl;
	}
	{
		cout << endl;
		my_auto_ptr<string> p(new string("hello"));
		my_auto_ptr<string> p1(new string("world"));
		cout << "values: " << *p << " " << *p1 << endl;
		p = new string("wonderful");
		cout << "values: " << *p << " " << *p1 << endl;
		p = p1 = new string("fuck");
		cout << "values: " << *p << " " << *p1 << endl;
	}
	{
		cout << endl;
		my_auto_ptr<tc> p(new tc(23));
		cout << p->Val() << endl;
		p = new tc(54);
		cout << p->Val() << endl;
	}
	return 0;
}