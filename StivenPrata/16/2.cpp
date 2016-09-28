#include <string>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <iterator>

using namespace std;

string replaceNotAlpha(const string& in)
{
	string out;
	copy_if(
		in.begin(), in.end(),
		insert_iterator<string>(out, out.begin()),
		[](char c) { return isalpha(c); }
	);
	return out;
}

string toUpper(const string& in)
{
	string out;
	transform(in.begin(), in.end(),
		insert_iterator<string>(out, out.begin()),
		[](char c){ return toupper(c); }
	);
	return out;
}

bool isPalyndrom(const string& str)
{
	auto clean = toUpper(replaceNotAlpha(str));

	auto b = clean.begin();
	auto e = clean.end();
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
	cout << replaceNotAlpha("Madam, I'm Adam") << endl;
	cout << toUpper("Madam, I'm Adam") << endl;

	cout << isPalyndrom("otto") << endl
		 << isPalyndrom("tot") << endl
		 << isPalyndrom("Madam, I'm Adam") << endl
		 << isPalyndrom("а роза упала на лапу азора") << endl
		 << isPalyndrom("a") << endl
		 << isPalyndrom("aa") << endl
		 << isPalyndrom("bugab") << endl;
	return 0;
}