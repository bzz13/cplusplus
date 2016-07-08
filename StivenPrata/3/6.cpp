#include <iostream>
using namespace std;

double lightYearsToAstroUnits(double lightYears)
{
	return lightYears * 63240;
}

int main()
{
	int distance, fuel;
	cout << "Введите пробег в км: ";
	cin >> distance;
	cout << "Введите объем израсходованного топлива в литрах: ";
	cin >> fuel;
	cout << "Средний расход " << fuel * 100.0 / distance << " литров на 100 км" << endl;
	return 0;
}