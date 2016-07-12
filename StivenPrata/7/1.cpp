#include <iostream>
using namespace std;

double calcMedian(double x, double y)
{
	return 2.0 * x * y / (x + y);
}

int main()
{
	double x,y;
	while(true)
	{
		cout << "enter x and y: ";
		cin >> x >> y;
		if (x && y)
			cout << "val: " << calcMedian(x,y) << endl;
		else
			break;
	}
	return 0;
}