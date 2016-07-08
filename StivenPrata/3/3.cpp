#include <iostream>
using namespace std;

double convertToDegrees(int degrees, int minutes, int seconds)
{
	const double SecondsInMinute = 60;
	const double MinutesInDegree = 60;

	return (seconds / SecondsInMinute + minutes) / MinutesInDegree + degrees;
}

int main()
{
	int degrees, minutes, seconds;
	cout << "Введите широту в градусах, минутах и секундах"<< endl;
	cout << "градусы: ";
	cin >> degrees;
	cout << "минуты: ";
	cin >> minutes;
	cout << "секунды: ";
	cin >> seconds;

	cout << degrees << " degrees "
		 << minutes << " minutes "
		 << seconds << " seconds "
		 << "= " << convertToDegrees(degrees, minutes, seconds)
		 << " degrees"
		 << endl;

	return 0;
}