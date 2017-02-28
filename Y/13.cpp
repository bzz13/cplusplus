#include <iostream>
#include <vector>
// #include <algorithm>

using namespace std;

int partition(vector<int>& data, int begin, int end)
{
    int pivot = data[end];
    int i = begin - 1;
    for(int j = begin; j < end; ++j)
    {
        if (data[j] <= pivot)
        {
            ++i;
            std::swap(data[i], data[j]);
        }
    }
    std::swap(data[i+1], data[end]);
    return i+1;
}

void quickSort(vector<int>& data, int begin = -1, int end = -1)
{
    if (begin < 0)
    {
        begin = 0;
    }
    if (end < 0)
    {
        end = data.size() - 1;
    }

    if (begin >= end)
    {
        return;
    }
    int p = partition(data, begin, end);
    quickSort(data, begin, p-1);
    quickSort(data, p+1, end);
}

int main()
{
    vector<int> v = {3,4,5,2,1,8,4,3,2,8};
    for(auto i: v)
    {
        std::cout << i << " ";
    }
    cout << endl;
    quickSort(v);
    for(auto i: v)
    {
        std::cout << i << " ";
    }
    cout << endl;
    return 0;
}