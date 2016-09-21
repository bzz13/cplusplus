#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <stdexcept>
using namespace std;

class bad_hmean: public logic_error
{
public:
	bad_hmean (const char * cstr): logic_error(cstr) { }
	bad_hmean (const string & str): logic_error(str) { }
};

class bad_gmean: public logic_error
{
public:
	bad_gmean (const char * cstr): logic_error(cstr) { }
	bad_gmean (const string & str): logic_error(str) { }
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
		catch (bad_hmean & bg)
		{
			cout << bg.what()
				 << "Try again." << endl;
			continue;
		}
		catch (bad_gmean & hg)
		{
			cout << hg.what()
				 << "Sorry, you don't get to play any more." << endl; // завершение работы
			break;
		}
	}
	cout << "Bye!" << endl;
	return 0;
}

double hmean(double a, double b)
{
	if (a == -b)
	{
		stringstream stream;
		stream	<< "hmean (" << a << ", " << b << "): " 
				<< "invalid arguments: a = -b" << endl;
		throw bad_hmean (stream.str());
	}
	return 2.0 * a * b / (a + b);
}
double gmean(double a, double b)
{
	if (a < 0 || b < 0)
	{
		stringstream stream;
		stream	<< "gmean(" << a << ", " << b << "): "
				<< "invalid arguments: should be >= 0" << endl;
		throw bad_gmean(stream.str());
	}
	return sqrt(a * b) ;
}