#include <iostream>
#include <string>

using namespace std;

const int MonthCount = 12;

int main()
{
	string month[MonthCount] {"янв", "фев", "мар", "апр", "май", "июн", "июл", "авг", "сен", "окт", "ноя", "дек"};
	int result = 0, sales[MonthCount] {};

	for (int i = 0; i < MonthCount; ++i)
	{
		cout << "Сколько продали в " << month[i] << " ? ";
		cin >> sales[i];
	}

	for (auto s: sales)
		result += s;

	cout << "за год " << result << endl;
}