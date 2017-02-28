#include <iostream>
#include <vector>
// #include <algorithm>

using namespace std;

void swap(std::vector<int>& data, int i, int j)
{
    auto tmp = data[i];
    data[i] = data[j];
    data[j] = tmp;
}

void swap_move(int& a, int& b)
{
    int c(std::move(a));
    a=std::move(b);
    b=std::move(c);
}


int partition(vector<int>& data, int begin, int end)
{
    int pivot = data[end];
    int i = begin - 1;
    for(int j = begin; j < end; ++j)
    {
        if (data[j] <= pivot)
        {
            ++i;
            // cout << "swap " << data[i] << " & " << data[j] << endl;
            // int& ir = data[i];
            // int& jr = data[j];
            // std::swap(ir, jr);
            // std::swap((int&)data[i],(int&)data[j]);
            std::swap(data[i], data[j]); //// ??????????????????????????????
            // swap_move(data[i], data[j]);
            // swap(data, i, j);
            // cout << "swaped " << data[i] << " & " << data[j] << endl;
        }
    }
    // cout << "swap " << data[i + 1] << " & " << data[end] << endl;

    // int& ir = data[i+1];
    // int& jr = data[end];
    // std::swap(ir, jr);
    // std::swap((int&)data[i+1],(int&)data[end]);
    std::swap(data[i+i], data[end]); //// ??????????????????????????????
    // swap_move(data[i+1], data[end]);
    // swap(data, i+1, end);
    // cout << "swaped " << data[i + 1] << " & " << data[end] << endl;

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
        end == data.size() - 1;
    }

    if (begin >= end)
    {
        return;
    }
    int p = partition(data, begin, end);
    // cout << p << endl;
    quickSort(data, begin, p-1);
    quickSort(data, p+1, end);
}

int main()
{
    vector<int> v = {3,4,5,2,1, 8,4,3,2,8};
    for(auto i: v)
    {
        std::cout << i << " ";
    }
    cout << endl;
    quickSort(v, 0, 9);
    for(auto i: v)
    {
        std::cout << i << " ";
    }
    cout << endl;
    return 0;
}