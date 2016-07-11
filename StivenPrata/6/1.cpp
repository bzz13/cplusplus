#include <iostream>
#include <cctype>
using namespace std;

int main()
{
	char c;
	while((c = cin.get()) != '@')
	{
		if (isdigit(c))
			continue;
		cout << char(islower(c) ? toupper(c) : tolower(c));
	}
	return 0;
}