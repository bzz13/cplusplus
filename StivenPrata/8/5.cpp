#include <iostream>
using namespace std;

const int MaxLength = 5;

template<typename T> T max(const T * ts)
{
	T result = ts[0];
	for (int i = 1; i < MaxLength; ++i)
	{
		if (ts[i] > result)
			result = ts[i];
	}
	return result;
}

int main()
{
	double ds[MaxLength] { 13.4, 21.0, 1, 43.5, 7.0 };
	int is[MaxLength] { 5, 2, 6, 7, 3 };

	cout << "doubles: " << max(ds) << endl;
	cout << "ints:    " << max(is) << endl;
}