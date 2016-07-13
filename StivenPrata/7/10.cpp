#include <iostream>
using namespace std;

double calculate(double x, double y, double (*func)(double, double));

double add(double x, double y)
{
	cout << "add ";
	return x + y;
}

double sub(double x, double y)
{
	cout << "sub ";
	return x - y;
}

double mul(double x, double y)
{
	cout << "mul ";
	return x * y;
}

double div(double x, double y)
{
	cout << "div ";
	return x / y;
}

const int OperationCount = 4;

typedef double (*f_ptr)(double, double);

int main()
{
	double (*funcs[OperationCount])(double, double) {add, sub, mul, div};
	f_ptr funcs2[OperationCount] {mul, div, add, sub};

	double x,y;
	while(cin >> x >> y)
	{
		cout << endl << "full def: " << endl;
		for (int i = 0; i < OperationCount; ++i)
		{
			cout << calculate(x,y, funcs[i]) << endl;
		}

		cout << endl << "type def: " << endl;
		for (int i = 0; i < OperationCount; ++i)
		{
			cout << calculate(x,y, funcs2[i]) << endl;
		}
	}


	return 0;
}

double calculate(double x, double y, double (*func)(double, double))
{
	return func(x, y);
}