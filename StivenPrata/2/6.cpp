#include <iostream>
using namespace std;

double lightYearsToAstroUnits(double lightYears)
{
	return lightYears * 63240;
}

int main()
{
	cout << "Enter the number of light years: ";
	double lightYears;
	cin >> lightYears;
	cout
		<< lightYears
		<< " light years = "
		<< lightYearsToAstroUnits(lightYears)
		<< " astronomical units" << endl;
	return 0;
}