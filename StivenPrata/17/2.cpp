#include <iostream>
#include <iomanip>
#include <fstream>

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "usage: ./o outfilename" << std::endl;
	}
	else
	{
		char c;
		std::ofstream out(argv[1]);
		while(std::cin.get(c))
		{
			out.put(c);
		}
		out << std::flush;
		out.close();
	}
	return 0;
}