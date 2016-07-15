#include <iostream>

template <typename T> // шаблон А
T SumArray(T arr[], int n);

template <typename T> // шаблон В
T SumArray(T * arr[], int n);

struct debts {
	char name[50];
	double amount;
};

int main()
{
	using namespace std;
	int things [6] = {13, 31, 103, 301, 310, 130}; 
	debts mr_E[3] =
	{
		{"Ima Wolfe", 2400.0},
		{"Ura Foxe", 1300.0},
		{"Iby Stout", 1800.0}
	};
	double * pd[3];
	// Установка указателей на члены amount структур в mr_E 
	for (int i = 0; i < 3; i++) 
		pd[i] = &mr_E[i].amount; 

	cout << "Listing Mr. E's counts of things:\n";
	// things - массив значений int
	int thingsSum = SumArray(things, 6); // использует шаблон A
	cout << "tempate inst call result: " << thingsSum << endl << endl;

	cout << "Listing Mr. E's debts: \n";
	// pd - массив указателей на double
	double pdSum = SumArray(pd, 3); // использует шаблон В (более специализированный)
	cout << "tempate inst call result: " << pdSum << endl;
	return 0;
}

template <typename T>
T SumArray(T arr[], int n)
{
	using namespace std; 
	cout << "template A\n There are " << n << " items with total sum: ";
	T result = arr[0];
	for (int i = 1; i < n; i++) 
		result += arr[i];
	cout << result << endl;
	return result;
}

template <typename T>
T SumArray (T * arr[], int n)
{
	using namespace std;
	cout << "template B\n There are " << n << " items with total sum: ";
	T result = *arr[0];
	for (int i = 1; i < n; i++) 
		result += *arr[i];
	cout << result << endl;
	return result;
}