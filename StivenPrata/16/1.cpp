#include <string>
#include <iostream>

bool isPalyndrom(const std::string& str)
{
	auto b = str.begin();
	auto e = str.end();
	--e;

	while(b < e)
	{
		if(*b != *e)
			return false;
		++b;
		--e;
	}
	return true;
}

int main()
{
	std::cout << isPalyndrom("otto") << std::endl
			  << isPalyndrom("tot") << std::endl
			  << isPalyndrom("madamimadam") << std::endl
			  << isPalyndrom("a") << std::endl
			  << isPalyndrom("aa") << std::endl
			  << isPalyndrom("bugab") << std::endl;
	return 0;
}