#include <iostream>
#include "golf.h"

int main()
{
	golf ann;
	setgolf(ann, "Ann Birdfree", 24);
	showgolf(ann);
	handicap(ann, 14);
	showgolf(ann);

	golf andy;
	std::cout << setgolf(andy) << std::endl;
	showgolf(andy);
}