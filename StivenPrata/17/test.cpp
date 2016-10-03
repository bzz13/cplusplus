#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
	if (argc == 3)
	{
		{
			ifstream ifs(argv[1]);
			string str;
			while(!ifs.eof())
			{
				ifs >> str;
				cout << str << endl;
			}
			ifs.close();
		}
		{
			ofstream ofs(argv[2]);
			if (ofs.is_open())
			{
				ofs << 1 << 2 << "\nhelp";
				ofs.close();
			}
		}
	}
	return 0;
}