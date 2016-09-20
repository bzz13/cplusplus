#include <iostream>
#include <exception>
#include <cstdlib>
#include <ctime>
#include <typeinfo>

using namespace std;

class grand
{
public:
	grand() { };
	virtual void say(){ cout << "I'm grand" << endl; }
};

class superb: public grand
{
	int v;
public:
	superb(int val): grand(), v(val) { };
	virtual void say() { cout << "I'm superb with val " << v << endl; }
	virtual void speak() { cout << "val is " << v << endl; }
};

class magnificent: public superb
{
	int c;
public:
	magnificent(int v, int ch): superb(v), c(ch) {};
	virtual void say() { cout << "I'm magnificent with c " << c << endl; }
	virtual void speak() { cout << "c is " << c << endl; }
};

grand * getOne()
{
	grand * p;
	switch(rand() % 3)
	{
		case 0: p = new grand(); break;
		case 1: p = new superb(rand()%100); break;
		case 2: p = new magnificent(rand()%100, rand()%100); break;
	}
	return p;
}

int main()
{
	grand * pg;
	for (int i = 0; i < 10; ++i)
	{
		pg = getOne();
		cout << "handling: " << typeid(*pg).name() << endl;

		pg->say();
		auto ps = dynamic_cast<superb*>(pg);
		if (ps != nullptr)
			ps->speak();

		cout << endl;
		delete pg;
	}

	return 0;
}
