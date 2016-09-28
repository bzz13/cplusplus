#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

template <typename T>
int reduce(T ar[], const int n)
{
	for_each(ar, ar + n, [](const T& t){ cout << t << " ";} ); cout << endl;

	sort(ar, ar + n);
	for_each(ar, ar + n, [](const T& t){ cout << t << " ";} ); cout << endl;

	auto end = unique(ar, ar + n);
	for_each(ar, end, [](const T& t){ cout << t << " ";}); cout << endl;

	return end - ar;
}

int main()
{
	long ldata[] = { 5, 7, 3, 4, 7, 3, 1, 2, 9, 2 };
	cout << reduce(ldata, 10) << endl;

	string sdata[] = {"asd", "cat", "bad", "asd", "sad"};
	cout << reduce(sdata, 5) << endl;
	return 0;
}