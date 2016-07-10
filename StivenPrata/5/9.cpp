#include <iostream>
#include <string>
using namespace std;

const int MaxLenght = 20;

int main()
{
	int count = 0;
	string result = "";
	
	while (true)
	{
		cin >> result;
		if (result == "done")
			break;
		count ++;
		cout << result << " ";
	}

	cout << endl << count;
	return 0;
}