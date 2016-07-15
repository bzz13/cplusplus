#include <iostream>
#include <cstring>
using namespace std;


template<typename T> T maxn(const T ts[], unsigned int count)
{
	T result = ts[0];
	for (int i = 1; i < count; ++i)
	{
		if (ts[i] > result)
			result = ts[i];
	}
	return result;
}

template<typename T> const T * maxn(const T * ts[], unsigned int count)
{
	cout << "spec char* version --> ";
	const char* result = ts[0];
	int resultlen = strlen(result);

	for (int i = 1; i < count; ++i)
	{
		auto len = strlen(ts[i]);
		if (len > resultlen)
		{
			resultlen = len;
			result = ts[i];
		}
	}
	return result;
}

int main()
{
	// template int max<int>(const int * ts, unsigned int); // явное создание экземпляра шаблона для Т=инт почему-то не заработало :(

	double ds[] { 13.4, 21.0, 1, 43.5 };
	int is[] { 5, 2, 6, 7, 3, 1 };
	const char* cs[] { "Ищу", "самое", "длинное", "слово" };

	cout << "doubles: " << maxn(ds, 4) << endl; // неявное создание экземпляра шаблона для Т=double
	cout << "ints:    " << maxn(is, 6) << endl; // тут я хотел вызов созданного экземпляра
	cout << "chars:   " << maxn(cs, 4) << endl; // явная специализация для T=char (в книжке сказано "более специализированное")
}