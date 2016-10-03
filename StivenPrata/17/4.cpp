#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		std::cerr << "usage: ./o infilename1 infilename2 outfilename" << std::endl;
	}
	else
	{
		std::ifstream in1(argv[1]);
		std::ifstream in2(argv[2]);
		if(in1.is_open() && in2.is_open())
		{
			std::ofstream out(argv[3]);
			std::string s1, s2;
			while(!in1.eof() && !in2.eof())
			{
				getline(in1, s1, '\n');
				getline(in2, s2, '\n');
				if(s1[s1.length() - 1] == '\r')
					s1 = s1.erase(s1.length() - 1,1);

				out << s1 << " " << s2 << "\n";
			}
			std::ifstream* tail = !in1.eof() ? &in1 : &in2;
			while(!tail->eof())
			{
				getline(*tail, s1);
				out << s1 << "\n";
			}
			out.flush();
			out.close();
			in1.close();
			in2.close();
		}
		else
		{
			std::cerr << "It's look like file " << argv[1] << " or " << argv[2] << " does not exit" << std::endl;
		}

	}
	return 0;
}