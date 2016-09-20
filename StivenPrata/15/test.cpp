#include <iostream>
#include <exception>
#include <cstdlib>
#include <ctime>
#include <typeinfo>

using namespace std;

class Analyzer;
class Probe
{
	int v;
public:
	Probe(int _v = 0): v(_v) { };
	friend void sync(Analyzer& a, const Probe& p);
	friend void sync(Probe& p, const Analyzer& a);

	friend ostream& operator<<(ostream& os, const Probe& a);
};
class Analyzer
{
	int v;
public:
	Analyzer(int _v = 0): v(_v) { };
	friend void sync(Probe& p, const Analyzer& a);
	friend void sync(Analyzer& a, const Probe& p);

	friend ostream& operator<<(ostream& os, const Analyzer& p);
};
 
inline ostream & operator<<(ostream& os, const Analyzer &a)
{
	return os << "Analyzer: " << a.v << endl;
}

inline ostream& operator<<(ostream& os, const Probe& p)
{
	return os << "Probe: " << p.v << endl;
}

class myException: public exception
{
public:
	int code;
	myException(int c = 0): exception(), code(c) { };
	virtual const char* what() const noexcept { return "myException"; };
};

inline void sync(Probe& p, const Analyzer& a) { if (p.v == 0) throw myException(a.v); p.v = a.v; }
inline void sync(Analyzer& a, const Probe& p) { a.v = p.v; }

int main()
{
	Analyzer a(432);
	Probe p(0);

	cout << a << p << endl;
	try
	{
		sync(p, a);
	}
	catch(const exception& e)
	{
		cout << e.what() << endl;
	}
	cout << a << p;

	return 0;
}
