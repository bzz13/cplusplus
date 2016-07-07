#include <iostream>
using namespace std;

float celsiusToFahrenheit(int degrees)
{
	return degrees * 1.8 + 32;
}

int main()
{
	cout << "Please enter s Celsius value: ";
	int degrees;
	cin >> degrees;
	cout
		<< degrees
		<< " degrees Celsius is "
		<< celsiusToFahrenheit(degrees)
		<< " degrees Fahrenheit" << endl;
	return 0;
}