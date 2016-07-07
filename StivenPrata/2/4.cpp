#include <iostream>
using namespace std;

int yearsToMonthConverter(int years)
{
	return years * 12;
}

int main()
{
	cout << "Enter your age: ";
	int years;
	cin >> years;
	cout << "Your age in month is " << yearsToMonthConverter(years) << endl;
	return 0;
}