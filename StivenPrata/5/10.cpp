#include <iostream>

using namespace std;

int main()
{
	int count;
	cin >> count;

	for (int y = 0; y < count; ++y)
	{
		for (int x = 0; x < count; ++x)
		{
			cout << (count - x - 1 > y ? '.' : '*');
		}
		cout << endl;
	}

	return 0;
}