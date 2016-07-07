#include <iostream>
using namespace std;

long long farlongsToYardsConverter(long long farlong)
{
	return farlong * 220;
}

long long farlongsToMetrsConverter(long long farlong)
{
	return farlong * 201168;
}

int main()
{
	cout << "input length in farlong: " << endl;
	long long farlong;
	cin >> farlong;

	cout << farlong << " farlongs is " << farlongsToYardsConverter(farlong) << " yards" << endl;
	cout << farlong << " farlongs is " << farlongsToMetrsConverter(farlong) << " metrs" << endl;
	return 0;
}