#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

int main()
{
	vector<int> a = {1,2,3,4,5,6};
	vector<int> b = {7,8,9,10,11};

	copy(b.begin(), b.end(), insert_iterator<vector<int>>(a, a.end()));
	ostream_iterator<int> out(cout, " ");
	copy(a.begin(), a.end(), out);
	return 0;
}