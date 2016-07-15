#include <iostream>
#include <string>
#include <cctype>
using namespace std;

const string & MyStringToUpperCase(string &);

const char * enter = "Enter a string (q to quit): ";

int main()
{
	string input;
	cout << enter;
	while(getline(cin, input))
	{
		if (input == "q")
		{
			cout << "Bye.";
			break;
		}
		cout << MyStringToUpperCase(input) << endl << enter;
	}
	return 0;
}

const string & MyStringToUpperCase(string & input)
{
	for(auto & c: input)
		c = toupper(c);
	return input;
}