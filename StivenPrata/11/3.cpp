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
	fout.open("3.cpp.out.txt");

	srand(time(0));// начальное значение для генератора случайных чисел

	int expCount;
	cout << "Enter experiments count: ";
	cin >> expCount;

	double target;
	cout << "Enter target distance (q to quit): ";
	cin >> target;

	double dstep;
	cout << "Enter step length: ";
	cin >> dstep;

	int * results = new int[expCount];

	for (int i = 0; i < expCount; ++i)
	{
		double direction;
		Vector step;
		Vector result(0.0, 0.0);
		unsigned long steps = 0;
		
	// Ввод заданного расстояния (q для завершения) 

		fout << "Target Distance: " << target << ", Step Size: " << dstep << endl;

		while (result.magval() < target)
		{
			direction = rand() % 360;
			step.reset(dstep, direction, Vector::POL);
			result = result + step;
			fout << steps << ": " << result << endl;
			steps++;
		}

		fout << "After " << steps << " steps, the subject has the following location:\n";
		fout << result << endl;
		result.polar_mode();
		fout << " or\n" << result << endl;
		fout << "Average outward distance per step = " << result.magval() / steps << endl;
		result.reset (0.0, 0.0);

		results[i] = steps;
	}

	int max, min, sum = 0, avg;
	max = min = sum = results[0];
	fout << "Exp count: " << expCount << endl
		 << "Exp results: " << max;
	for (int i = 1; i < expCount; ++i)
	{
		if (max < results[i])
			max = results[i];
		else if (min > results[i])
			min = results[i];
		sum += results[i];
		fout << ", " << results[i];
	}
	avg = sum / expCount;

	cout << "Exp count: " << expCount << endl
		 << "Min step count: " << min << endl
		 << "Max step count: " << max << endl
		 << "Avg step count: " << avg << endl;

	fout << endl
		 << "Min step count: " << min << endl
		 << "Max step count: " << max << endl
		 << "Avg step count: " << avg << endl;



	cout << "Bye!\n";
	cin.clear();
	while (cin.get() != '\n')
		continue;
	fout.close();
	delete[] results;
	return 0;
}