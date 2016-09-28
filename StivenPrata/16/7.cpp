#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

vector<int> Lotto(const int& total, const int& size)
{
	if(size > total)
		throw out_of_range("size must be less or equal than total");
	vector<int> all;
	for (int i = 1; i <= total; ++i)
		all.push_back(i);

	random_shuffle(all.begin(), all.end());
	all.resize(size);
	sort(all.begin(), all.end());
	return all;
}

int main()
{
	vector<int> winners = Lotto(51, 6);
	cout << "winners is ";
	copy(winners.begin(), winners.end(), ostream_iterator<int>(cout, " "));
	return 0;
}