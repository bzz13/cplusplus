#include <iostream>
#include "sales.h"

namespace SALES
{
	// Копирует меньшее значение из 4 или п элементов из массива 
	// аг в член sales структуры s, вычисляет и сохраняет 
	// среднее арифметическое, максимальное и минимальное 
	// значения введенных чисел;
	// оставшиеся элементы sales, если таковые есть, устанавливаются в 0 
	void setSales (Sales & s, const double ar[], int n)
	{
		for (int i = 0; i < n && i < QUARTERS; ++i)
			s.sales[i] = ar[i];
		for (int i = n; i < QUARTERS; ++i)
			s.sales[i] = 0.0;

		double sum;
		sum = s.min = s.max = s.sales[0];
		for (int i = 1; i < n; ++i)
		{
			if (s.sales[i] < s.min)
				s.min = s.sales[i];
			if (s.sales[i] > s.max)
				s.max = s.sales[i];
			sum += s.sales[i];
		}

		s.average = sum / n;
	}

	// Интерактивно подсчитывает продажи за 4 квартала,
	// сохраняет их в члене sales структуры s, вычисляет и 
	// сохраняет среднее арифметическое, а также максимальное 
	// и минимальное значения введенных чисел 
	void setSales(Sales & s)
	{
		using namespace std;
		int readed = 0;
		double d[QUARTERS]{};

		for (int i = 0; i < QUARTERS; ++i, ++readed)
		{
			cout << "Enter sale for " << (i+1) << " quarter: ";
			if (!(cin >> d[i]))
				break;
		}

		setSales(s, d, readed);
	}

	// Отображает всю информацию из структуры s 
	void showSales (const Sales & s)
	{
		using std::cout;
		using std::endl;

		cout << "{ sales=[";
		for (int i = 0; i < QUARTERS; ++i)
		{
			if (i) cout << ", ";;
			cout << s.sales[i];
		}
		cout << "], min=\"" << s.min << "\", max=\"" << s.max << "\", av=\"" << s.average << "\" }" << endl;
	}
}