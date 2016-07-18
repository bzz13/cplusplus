#include <iostream>
#include "golf.h"

int main()
{
	golf ann("Ann Birdfree", 24);
	ann.show();
	ann.sethandicap(14);
	ann.show();

	golf andy;
	andy.show();
}