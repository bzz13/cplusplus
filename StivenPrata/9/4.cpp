#include "sales.h"

int main()
{
	SALES::Sales s1, s2, s3{};

	SALES::showSales(s3);

	SALES::setSales(s1);
	SALES::showSales(s1);

	using namespace SALES;
	double ds1[SALES::QUARTERS]{4.0, 6.5, 2.5, 5.0};
	setSales(s2, ds1, SALES::QUARTERS);
	showSales(s2);

	double ds2[3]{4.0, 6.5, 2.5};
	setSales(s2, ds1, 3);
	showSales(s2);

	return 0;
}