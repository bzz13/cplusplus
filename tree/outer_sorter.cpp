#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <random>
#include <queue>
#include <stdio.h>
#include <memory>

using namespace std;

template<typename T>
class outer_sorter
{
    size_t max_item_count;
    vector<string> split(ifstream& fin)
    {
        istream_iterator<T> eos;
        istream_iterator<T> iit(fin);
        vector<string> result;

        while(iit != eos)
        {
            string tmp_file_name(to_string(result.size() + 1) + ".txt");
            ofstream fout(tmp_file_name.c_str());
            size_t readed(0);
            vector<T> data;
            while(iit != eos && readed < max_item_count)
            {
                data.push_back(*iit);
                ++iit;
                ++readed;
            }
            std::sort(data.begin(), data.end());
            for(auto t: data)
            {
                fout << t << endl;
            }
            fout.close();
            result.push_back(tmp_file_name);
        }

        return result;
    }

    void merge(vector<string> fins, ofstream& fout)
    {
        class reader
        {
            ifstream fin;
            T t;
        public:
            reader(const reader&) = delete;
            reader(const string& ifstream_name): fin(ifstream_name.c_str())
            {
                fin >> t;
            }
            ~reader()
            {
                fin.close();
            }

            bool has_next()
            {
                fin >> t;
                return !fin.eof();
            }

            const T current() const
            {
                return t;
            }
        };

        struct comparer
        { 
            bool operator() (const shared_ptr<reader>& lhs, const shared_ptr<reader>& rhs) const
            {
                return (lhs->current() > rhs->current());
            }
        };

        vector<shared_ptr<reader>> readers;
        for(auto fname: fins)
        {
            readers.push_back(make_shared<reader>(fname));
        }

        priority_queue<shared_ptr<reader>, vector<shared_ptr<reader>>, comparer> queue(readers.begin(), readers.end());
        while(queue.size() > 0)
        {
            auto top = queue.top();
            queue.pop();
            fout << top->current() << endl;
            if (top->has_next())
            {
                queue.push(top);
            }
        }
    }

    void clean(vector<string> fins)
    {
        for(auto f: fins)
        {
            remove(f.c_str());
        }
    }

public:
    outer_sorter(size_t max): max_item_count(max) { }

    void sort(const string& fin_name, const string& fout_name)
    {
        ifstream fin(fin_name.c_str());
        ofstream fout(fout_name.c_str());

        auto sorted_streams = split(fin);
        merge(sorted_streams, fout);
        clean(sorted_streams);

        fin.close();
        fout.close();
    }
};

vector<int> prepare(string fname, size_t size)
{
    default_random_engine generator;
    uniform_int_distribution<int> distribution(0, 10000);
    vector<int> result;
    for(size_t i = 0; i < size; ++i)
    {
        result.push_back(distribution(generator));
    }

    ofstream fout(fname.c_str());
    for(auto i: result)
    {
        fout << i << endl;
    }
    fout.close();

    return result;
}

template<typename T>
bool check(vector<T> expected, const string& check_fin_name)
{
    sort(expected.begin(), expected.end());
    vector<T> actual;
    ifstream fin(check_fin_name.c_str());
    istream_iterator<T> iit(fin);
    istream_iterator<T> eos;
    while(iit != eos)
    {
        actual.push_back(*iit);
        iit++;
    }
    fin.close();

    if (expected.size() != actual.size())
    {
        cout << expected.size() << " but was " << actual.size() << endl;
        return false;
    }

    for (int i = 0; i < expected.size(); ++i)
    {
        if (expected[i] != actual[i])
        {
            return false;
        }
    }
    return true;
}

int main()
{
    auto nums = prepare("in.txt", 990);

    outer_sorter<int> s(250);
    s.sort("in.txt", "out.txt");

    cout << (check(nums, "out.txt") ? "success" : "fail") << endl;

    return 0;
}