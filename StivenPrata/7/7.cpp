#include <iostream>
using namespace std;

const int Limit = 10;

double * Fill_array(double *, int);
void Show_array(const double *, const double *);
void Reverse_array(double *, double *);

int main()
{
	double * arr = new double[Limit];
	double * afterLastElement = Fill_array(arr, Limit);
	cout << "before reverse" << endl;
	Show_array(arr, afterLastElement);

	Reverse_array(arr+1, afterLastElement - 1);
	cout << "before reverse" << endl;
	Show_array(arr, afterLastElement);

	delete [] arr;
	return 0;
}

double * Fill_array(double * ds, int count)
{
	int readed = 0;
	for (int i = 0; i < count; ++i, ++readed)
	{
		cout << "enter val of " << (i+1) << " double ( q - for quit) : ";
		if (!(cin >> ds[i]))
			break;
	}
	return ds + readed;
}
void Show_array(const double * begin, const double * end)
{
	cout << "Values: ";
	for (const double * i = begin; i < end; ++i)
		cout << *i << " ";
	cout << endl;
}
void Reverse_array(double * begin, double * end)
{
	for (double * i = begin, * j = end - 1; i < j; ++i, --j)
	{
		double tmp = *i;
		*i = *j;
		*j = tmp;
	}
}