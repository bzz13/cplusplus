#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstdio>

using namespace std;

template<typename T>
class outerSorter
{
	int partsCounter;

	string writePartToFile(const vector<T>& part)
	{
		string ofstreamName(to_string(++partsCounter) + ".txt");
		ofstream os;
		os.open(ofstreamName, ofstream::out);
		ostream_iterator<T> out_it (os, "\n");
		copy(part.begin(), part.end(), out_it);
		os.close();
		return ofstreamName;
	}

	vector<string> splitIntoOrderedParts(const string& inFilepath, const int& max)
	{
		ifstream is;
		is.open(inFilepath);

		vector<string> partNames;
		istream_iterator<T> iter(is);
		istream_iterator<T> eos;

		while(iter != eos)
		{
			vector<T> part;
			int count = 0;
			while(count < max && iter != eos)
			{
				part.push_back(*iter);
				++ iter;
				++ count;
			}
			std::sort(part.begin(), part.end());
			partNames.push_back(writePartToFile(part));
		}

		is.close();
		return partNames;
	}

	string mergeTwoParts(const string& partName1, const string& partName2)
	{
		ifstream is1; is1.open(partName1);
		ifstream is2; is2.open(partName2);

		string mergedPartName(to_string(++partsCounter) + ".txt");
		ofstream os;
		os.open(mergedPartName);

		istream_iterator<T> iter1(is1);
		istream_iterator<T> iter2(is2);
		istream_iterator<T> eos;

		while(iter1 != eos && iter2 != eos)
		{
			if (*iter1 < *iter2)
			{
				os << *iter1 << endl;
				++iter1;
			}
			else
			{
				os << *iter2 << endl;
				++iter2;
			}
		}
		auto iter = iter1 != eos ? iter1 : iter2;
		while(iter != eos)
		{
			os << *iter << endl;
			++iter;
		}

		is1.close();
		is2.close();
		os.close();
		remove(partName1.c_str());
		remove(partName2.c_str());

		return mergedPartName;
	}

	string joinOrderedParts(const vector<string>& partNames)
	{
		queue<string> q;
		for (auto partName: partNames)
			q.push(partName);
		while(q.size() > 1)
		{
			auto p1 = q.front(); q.pop();
			auto p2 = q.front(); q.pop();
			q.push(mergeTwoParts(p1, p2));
		}
		return q.front();
	}

	string joinOrderedPartsWithPriorityQueue(const vector<string>& partNames)
	{
		class reader
		{
			ifstream is;
			istream_iterator<T> iter;
			istream_iterator<T> eos;
		public:
			reader(const string& name) { is.open(name); iter = istream_iterator<T>(is); }
			~reader() { is.close(); }
			bool hasNext() const { return iter != eos; }
			const T& current() const { return *iter; }
			void readNext() { ++iter; }
		};

		string ofstreamName(to_string(++partsCounter) + ".txt");
		ofstream os;
		os.open(ofstreamName, ofstream::out);

		struct LessThanByCurrent
		{
			bool operator()(const reader* lhs, const reader* rhs) const
			{
				return lhs->current() > rhs->current();
			}
		};

		priority_queue<reader*, vector<reader*>, LessThanByCurrent> pq;
		for(auto partName: partNames)
			pq.push(new reader(partName));

		while(!pq.empty())
		{
			auto topReader = pq.top(); pq.pop();
			if (topReader->hasNext())
			{
				os << topReader->current() << endl;
				topReader->readNext();
				pq.push(topReader);
			}
			else
			{
				delete topReader;
			}
		}

		os.close();

		for(auto partName: partNames)
			remove(partName.c_str());

		return ofstreamName;
	}

public:
	string sort(string inFilepath, int max = 10)
	{
		partsCounter = 0;
		return
			// joinOrderedParts(
			joinOrderedPartsWithPriorityQueue(
				splitIntoOrderedParts(inFilepath, max));
	}
};

int main()
{
	outerSorter<int> sorter;
	cout << sorter.sort("input.txt");
}