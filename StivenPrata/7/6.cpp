#include <iostream>
using namespace std;

const int Limit = 10;

int Fill_array(double *, int);
void Show_array(const double *, int);
void Reverse_array(double *, int);

int main()
{
	double arr[Limit]{};
	int count = Fill_array(arr, Limit);
	cout << "before reverse" << endl;
	Show_array(arr, count);

	Reverse_array(arr+1, count - 2);
	cout << "before reverse" << endl;
	Show_array(arr, count);

	return 0;
}

int Fill_array(double * ds, int count)
{
	int readed = 0;
	for (int i = 0; i < count; ++i, ++readed)
	{
		cout << "enter val of " << (i+1) << " double: ";
		if (!(cin >> ds[i]))
			break;
	}
	return readed;
}
void Show_array(const double * ds, int count)
{
	cout << "Values: ";
	for (int i = 0; i < count; ++i)
		cout << ds[i] << " ";
	cout << endl;
}
void Reverse_array(double * ds, int count)
{
	for (int i = 0, j = count - 1; i < j; ++i, --j)
	{
		double tmp = ds[i];
		ds[i] = ds[j];
		ds[j] = tmp;
	}
}