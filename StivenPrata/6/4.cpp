#include <iostream>
#include <string>
using namespace std;

void printHelp()
{
	cout << "Please enter one of the following choices:" << endl
		 << "a) by name" << endl
		 << "b) by nick" << endl
		 << "c) by title" << endl
		 << "d) by preference" << endl
		 << "h) help" << endl
		 << "q) quit" << endl;
}

void printErr()
{
	cout << "incorrect input. type 'h' for help" << endl;
}

enum preference { name = 0, nick = 1, title = 2 };

struct bop
{
	string name;
	string nick;
	string title;
	preference pref;
};

int main()
{
	printHelp();

	bop collection[3]
	{
		{"Bob Marly", "марля", "director", nick},
		{"Bob Dilan", "bobby", "gamer", title},
		{"Fredy Mercury", "крюгер", "member", name},
	};

	bool complete = false;
	while(!complete)
	{
		char c;
		cin >> c;
		switch (c)
		{
			case 'a':
				for (auto m: collection)
					cout << m.name << endl;
				break;
			case 'b':
				for (auto m: collection)
					cout << m.nick << endl;
				break;
			case 'c':
				for (auto m: collection)
					cout << m.title << endl;
				break;
			case 'd':
				for (auto m: collection)
				{
					switch (m.pref)
					{
						case name:
							cout << m.name << endl;
							break;
						case nick:
							cout << m.nick << endl;
							break;
						case title:
							cout << m.title << endl;
							break;
					}
				}
				break;
			case 'h':
				printHelp();
				break;
			case 'q':
				complete = true;
				break;
			default:
				printErr();
				break;
		}
	}
	return 0;
}