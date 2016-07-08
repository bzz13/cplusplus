#include <iostream>
using namespace std;

const double Gallon = 3.875;
const double Mile = 1.60934;

void printUSStyle(double euroStyle)
{
	cout << "Средний расход в формате США " << ( 100 / euroStyle * Gallon / Mile) << " миль на галлон" << endl;
}

int main()
{
	cout << "Введите средний расход топлива литров на 100 км" << endl;
	double euroStyle;
	cin >> euroStyle;
	printUSStyle(euroStyle);
	return 0;
}