#include <iostream>
#include <string>

using namespace std;

const int MonthCount = 12, YearsCount = 3;

int main()
{
	string month[MonthCount] {"янв", "фев", "мар", "апр", "май", "июн", "июл", "авг", "сен", "окт", "ноя", "дек"};
	int result = 0, sales[YearsCount][MonthCount] {};

	for (int y = 0; y < YearsCount; ++y)
	{
		cout << endl;
		for (int m = 0; m < MonthCount; ++m)
		{
			cout << "Сколько продали в " << month[m] << " ? ";
			cin >> sales[y][m];
		}
	}

	for (int y = 0; y < YearsCount; ++y)
	{
		int forYear = 0;
		for (int m = 0; m < MonthCount; ++m)
			forYear += sales[y][m];

		cout << "за " << y << " год " << forYear << endl;
		result += forYear;
	}
	cout << "за " << YearsCount << " год " << result << endl;
}