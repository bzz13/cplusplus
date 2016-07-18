#include "sales.h"

int main()
{
	SALES::Sales s1;
	s1.show();

	using namespace SALES;
	double ds1[SALES::QUARTERS]{4.0, 6.5, 2.5, 5.0};
	Sales s2(ds1, SALES::QUARTERS);
	s2.show();

	double ds2[3]{4, 6, 2};
	Sales s3(ds2, 3);
	s3.show();

	return 0;
}