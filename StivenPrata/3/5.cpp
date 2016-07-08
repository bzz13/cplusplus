#include <iostream>
using namespace std;

void printPercentages(long long worldPopulation, long long countryPopulation)
{
	auto perc = countryPopulation * 100.0L / worldPopulation;
	cout << "Country population is " << perc << "%" << endl;
}

int main()
{
	long long worldPopulation, countryPopulation;
	cout << "Enter world population: ";
	cin >> worldPopulation;
	cout << "Enter cntry population: ";
	cin >> countryPopulation;
	printPercentages(worldPopulation, countryPopulation);
	return 0;
}