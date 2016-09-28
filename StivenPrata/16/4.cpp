#include <algorithm>
#include <iostream>

using namespace std;

int reduce(long ar[], int n)
{
	sort(ar, ar + n);
	for_each(ar, ar + n, [](long l){cout << l << " ";}); cout << endl;

	auto end = unique(ar, ar + n);
	for_each(ar, end, [](long l){cout << l << " ";}); cout << endl;

	return end - ar;
}

int main()
{
	long data[10] = {5, 7, 3, 4, 7, 3, 1, 2, 9, 2};
	cout << reduce(data, 10);
	return 0;
}