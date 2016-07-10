#include <iostream>
#include <cstring>
using namespace std;

const int MaxLenght = 20;

int main()
{
	int count = 0;
	char result[MaxLenght] { '\0' };
	
	while (true)
	{
		cin >> result;
		if (!strcmp(result, "done"))
			break;
		count ++;
		cout << result << " ";
	}

	cout << endl << count;
	return 0;
}