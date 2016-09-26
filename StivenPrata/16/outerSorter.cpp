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
class reader
{
	ifstream is;
	istream_iterator<T> iter;
	istream_iterator<T> eos;
	reader(const reader&);
public:
	reader(const string& name) { is.open(name); iter = istream_iterator<T>(is); }
	~reader() { is.close(); }
	bool hasNext() const { return iter != eos; }
	const T& current() const { return *iter; }
	void readNext() { ++iter; }
};

template<typename T>
class writer
{
	ofstream os;
	writer(const writer&);
public:
	writer(const string& name) { os.open(name, ofstream::out); }
	~writer() { os.close(); }
	void write(const T& t) { os << t << endl; }
	void write(const vector<T>& v) {
		ostream_iterator<T> out_it (os, "\n");
		copy(v.begin(), v.end(), out_it);
	}
};

template<typename T>
struct LessThanByCurrent
{
	bool operator()(const reader<T>* lhs, const reader<T>* rhs) const
	{
		return lhs->current() > rhs->current();
	}
};

template<typename T>
class outerSorter
{
	int partsCounter;

	string writePartToFile(const vector<T>& part)
	{
		string ofstreamName(to_string(++partsCounter) + ".txt");
		writer<T> wr(ofstreamName);
		wr.write(part);
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
		reader<T> r1(partName1);
		reader<T> r2(partName2);

		string mergedPartName(to_string(++partsCounter) + ".txt");
		writer<T> wr(mergedPartName);

		while(r1.hasNext() && r2.hasNext())
		{
			if (r1.current() < r2.current())
			{
				wr.write(r1.current());
				r1.readNext();
			}
			else
			{
				wr.write(r2.current());
				r2.readNext();
			}
		}
		reader<T>* r = r1.hasNext() ? &r1 : &r2;
		while(r->hasNext())
		{
			wr.write(r->current());
			r->readNext();
		}

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
		priority_queue<reader<T>*, vector<reader<T>*>, LessThanByCurrent<T> > pq;
		for(auto partName: partNames)
			pq.push(new reader<T>(partName));

		string ofstreamName(to_string(++partsCounter) + ".txt");
		writer<T> wr(ofstreamName);
		while(!pq.empty())
		{
			auto topReader = pq.top(); pq.pop();
			if (topReader->hasNext())
			{
				wr.write(topReader->current());
				topReader->readNext();
				pq.push(topReader);
			}
			else
			{
				delete topReader;
			}
		}

		for(auto partName: partNames)
			remove(partName.c_str());

		return ofstreamName;
	}

public:
	string sort(string inFilepath, const int& max = 10, const bool& usePriorityQueue = false )
	{
		partsCounter = 0;
		auto parts = splitIntoOrderedParts(inFilepath, max);
		return usePriorityQueue
				? joinOrderedPartsWithPriorityQueue(parts)
				: joinOrderedParts(parts);
	}
};

int main()
{
	outerSorter<int> sorter;
	cout << sorter.sort("input.txt", 4, true);
}