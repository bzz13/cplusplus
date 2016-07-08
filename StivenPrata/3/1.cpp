#include <iostream>
using namespace std;


int main()
{
	const auto inchCountInFoot = 12;

	int inches;

	cout << "Введите рост в дюймах: _____\b\b\b\b\b";
	cin >> inches;
	cout << "Ваш рост " << inches / inchCountInFoot << " футов";
	if (inches % inchCountInFoot)
		cout << " и " << inches % inchCountInFoot << " дюймов" << endl;

	return 0;
}