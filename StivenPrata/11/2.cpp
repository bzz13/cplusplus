#include <iostream>
#include <fstream>
#include <cstdlib> // прототипы rand(), srand()
#include <ctime> // прототип time()
#include "vect2.h"

int main()
{
	using namespace std;
	using VECTOR::Vector;

	ofstream fout;
	fout.open("2.cpp.out.txt");

	srand(time(0));// начальное значение для генератора случайных чисел
	double direction;
	Vector step;
	Vector result(0.0, 0.0);
	unsigned long steps = 0;
	double target;
	double dstep;
	cout << "Enter target distance (q to quit) : ";
// Ввод заданного расстояния (q для завершения) 
	while (cin >> target)
	{
		cout << "Enter step length: ";// ввод длины шага 
		if (! (cin >> dstep) )
			break;
		fout << "Target Distance: " << target << ", Step Size: " << dstep << endl;

		while (result.magval() < target)
		{
			direction = rand() % 360;
			step.reset(dstep, direction, Vector::POL);
			result = result + step;
			fout << steps << ": " << result << endl;
			steps++;
		}

		cout << "After " << steps << " steps, the subject has the following location:\n";
		fout << "After " << steps << " steps, the subject has the following location:\n";
		cout << result << endl;// вывод позиции после steps шагов
		fout << result << endl;
		result.polar_mode();
		cout << " or\n" << result << endl;
		fout << " or\n" << result << endl;
		cout << "Average outward distance per step = " << result.magval() / steps << endl; // вывод среднего расстояния на один шаг steps = 0;
		fout << "Average outward distance per step = " << result.magval() / steps << endl;
		result.reset (0.0, 0.0);
		cout << "Enter target distance (q to quit): "; // Ввод заданного расстояния (q для завершения)
		steps = 0;
	}
	cout << "Bye!\n";
	cin.clear();
	while (cin.get() != '\n')
		continue;
	fout.close();
	return 0;
}