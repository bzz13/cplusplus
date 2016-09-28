#include <algorithm>
#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <array>
#include <functional>
#include <iterator>

using namespace std;

double calcTime(function<void (void)> f)
{
	auto start = clock();
	f();
	auto end = clock();
	return (double) (end - start)/CLOCKS_PER_SEC;
}

int main()
{
	array<long,3> sizes = {100000, 1000000, 10000000};
	for (auto size: sizes)
	{
		cout << size << endl;
		// а
		vector<int> vi0(size);
		for(auto& v: vi0)
			v = rand();
		// б
		vector<int> vi(vi0);
		list<int> li(vi0.begin(), vi0.end());
		list<int> lii(vi0.begin(), vi0.end());
		// в
		cout << "sort vector: " << calcTime([&vi](){ sort(vi.begin(), vi.end()); }) << endl;
		cout << "sort list: " << calcTime([&li](){ li.sort(); }) << endl;
		// г
		cout << "sort-copy list: " << calcTime([&lii]{
			vector<int> v(lii.begin(), lii.end());
			sort(v.begin(), v.end());
			copy(v.begin(), v.end(), lii.begin());
		}) << endl;

		cout << endl;
	}
	
}