#include <iostream>
using namespace std;

class test
{
public:
	test(int v): val(v) { };
	virtual int func(int c) const { return val; }

	int val;
};

class exttest: public test
{
public:
	exttest(int v, int w): test(v), val(w){ };
	virtual int func() const { return val + test::val; }


	int val;
};

int main()
{
	exttest et(10, 17);

	et.func();
	et.test::func(5);

	// test& ret = et;
	// test* pet = &et;

	// cout << ret.func() << endl;
	// cout << pet->func() << endl;


	// cout << ret.test::func() << endl;
	// cout << pet->test::func() << endl;

	return 0;
}