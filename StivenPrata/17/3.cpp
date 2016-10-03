#include <iostream> 
#include <iomanip>
#include <fstream>

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cerr << "usage: ./o infilename outfilename" << std::endl;
	}
	else
	{
		std::ifstream in(argv[1]);
		if(in.is_open())
		{
			std::ofstream out(argv[2]);
			char c;
			while(in.get(c))
			{
				out.put(c);
			}
		}
		else
		{
			std::cerr << "It's look like file " << argv[1] << " does not exit" << std::endl;
		}
	}
	return 0;
}
