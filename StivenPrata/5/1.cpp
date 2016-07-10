#include <iostream>
using namespace std;

int main()
{
	int begin, end, result = 0;
	cin >> begin >> end;

	for(auto i = begin; i <= end; ++i)
		result += i;

	cout << result;
}