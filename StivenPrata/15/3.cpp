#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <stdexcept>
using namespace std;

class my_exception: public logic_error
{
	string fname;
	double v1;
	double v2;

public:
	my_exception(const char* s, double a, double b):
		logic_error(""), fname(s), v1(a), v2(b) { }

	virtual const char * what() {
		stringstream stream;
		stream	<< fname << "(" << v1 << ", " << v2 << "): "
		        << "invalid arguments: should be >= 0" << endl;
		return stream.str().c_str();
	}
};

class bad_hmean: public my_exception
{
public:
	bad_hmean (double a, double b): my_exception("bad_hmean", a, b) { }
};

class bad_gmean: public my_exception
{
public:
	bad_gmean (double a, double b): my_exception("bad_gmean", a, b) { }
};

double hmean(double a, double b);
double gmean(double a, double b);

int main()
{
	double x, y, z;
	cout << "Enter two numbers: ";
	while (cin >> x >> y)
	{
		try
		{
			z = hmean(x, y);
			cout << "Harmonic mean of " << x << " and y " << y << " is " << z << endl;
			cout << "Geometric mean of " << x << " and y " << y << " is " << gmean(x, y) << endl;
			cout << "Enter next set of numbers <q to quit>: ";
		}
		catch (my_exception & me)
		{
			cout << me.what();
			break;
		}
	}
	cout << "Bye!" << endl;
	return 0;
}

double hmean(double a, double b)
{
	if (a == -b)
		throw bad_hmean (a,b);
	return 2.0 * a * b / (a + b);
}
double gmean(double a, double b)
{
	if (a < 0 || b < 0)
		throw bad_gmean(a, b);
	return sqrt(a * b) ;
}