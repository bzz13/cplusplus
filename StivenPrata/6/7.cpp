#include <iostream>
#include <string>
#include <cctype>
using namespace std;


int main()
{
	string str;
	unsigned int vowels, consonants, other;
	vowels = consonants = other = 0;
	while( cin >> str && str != "q")
	{
		char f = str[0];
		if (isalpha(f))
		{
			if (f == 'a' || f == 'e' || f == 'i' || f == 'o' || f == 'u' || f == 'y')
				vowels ++;
			else
				consonants ++;
		} else {
			other ++;
		}
	}
	cout << vowels << " words beginning with vowels" << endl
		 << consonants << " words beginning with consonants" << endl
		 << other << " others";
}