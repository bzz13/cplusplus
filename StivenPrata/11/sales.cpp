#include <iostream>
#include "sales.h"

namespace SALES
{
	void Sales::calc(int n)
	{
		double sum;
		sum = min = max = sales[0];
		for (int i = 1; i < n; ++i)
		{
			if (sales[i] < min)
				min = sales[i];
			if (sales[i] > max)
				max = sales[i];
			sum += sales[i];
		}

		average = sum / n;
	}

	// Копирует меньшее значение из 4 или п элементов из массива 
	// аг в член sales структуры s, вычисляет и сохраняет 
	// среднее арифметическое, максимальное и минимальное 
	// значения введенных чисел;
	// оставшиеся элементы sales, если таковые есть, устанавливаются в 0 
	Sales::Sales (const double ar[], int n)
	{
		for (int i = 0; i < n && i < QUARTERS; ++i)
			sales[i] = ar[i];
		for (int i = n; i < QUARTERS; ++i)
			sales[i] = 0.0;
		calc(n);
	}

	// Интерактивно подсчитывает продажи за 4 квартала,
	// сохраняет их в члене sales структуры s, вычисляет и 
	// сохраняет среднее арифметическое, а также максимальное 
	// и минимальное значения введенных чисел 
	Sales::Sales()
	{
		using namespace std;
		int readed = 0;
		double d[QUARTERS]{};

		for (int i = 0; i < QUARTERS; ++i, ++readed)
		{
			cout << "Enter sale for " << (i+1) << " quarter: ";
			if (!(cin >> sales[i]))
				break;
		}

		calc(readed);
	}

	// Отображает всю информацию из структуры s 
	void Sales::show() const
	{
		using namespace std;
		cout << "{ sales=[";
		for (int i = 0; i < QUARTERS; ++i)
		{
			if (i) cout << ", ";;
			cout << sales[i];
		}
		cout << "], min=\"" << min << "\", max=\"" << max << "\", av=\"" << average << "\" }" << endl;
	}
}