#include <iostream>
#include <array>
using namespace std;

const int arSize = 10;

int main()
{
	array<double, arSize> amounts {};

	int count = 0;
	for (int i = 0; i < arSize && cin >> amounts[i]; ++i, ++count);

	double avarege = 0;
	for (int i = 0; i < count; ++i)
		avarege += amounts[i];
	avarege /= count;

	cout << "avarege: " << avarege << endl;

	int moreThanAvrgCount = 0;
	for (int i = 0; i < count; ++i)
	{
		if (amounts[i] > avarege)
		{
			cout << amounts[i] << " ";
			moreThanAvrgCount ++;
		}
	}
	cout << "more than avarege: " << moreThanAvrgCount << " element" << endl;

	return 0;
}