#include <vector>
#include <iostream>

using namespace std;

template<typename T>
vector<T> merge(const vector<T>& a, const vector<T>& b)
{
    vector<T> c(a);
    c.insert(c.end(), b.begin(), b.end());

    int indexA = a.size() - 1;
    int indexB = b.size() - 1;
    int indexC = c.size() - 1;

    while(indexA >= 0 && indexB >= 0)
    {
        if (a[indexA] <= b[indexB])
        {
            c[indexC] = b[indexB];
            indexB --;
        }
        else
        {
            c[indexC] = a[indexA];
            indexA --;
        }
        indexC --;
    }

    while(indexB >= 0)
    {
        c[indexC] = b[indexB];
        indexB --;
        indexC --;
    }

    return c;
}


void printV(const vector<int>& a)
{
    for(auto v: a)
    {
        cout << v << " ";
    }
}

void print(const vector<int>& a, const vector<int>& b, const vector<int>& c)
{
    printV(a);
    cout << "+ ";
    printV(b);
    cout << "= ";
    printV(c);
    cout << endl;
}

void test1()
{
    vector<int> a{1, 4, 5 ,6};
    vector<int> b{1, 2, 3};
    vector<int> c = merge<int>(a, b);
    print(a, b, c);
}

void test2()
{
    vector<int> a{1, 4, 5 ,7};
    vector<int> b{1, 2, 6};
    vector<int> c = merge<int>(a, b);
    print(a, b, c);
}

void test3()
{
    vector<int> a{10, 20, 20 , 40};
    vector<int> b{1, 2, 3};
    vector<int> c = merge<int>(a, b);
    print(a, b, c);
}

void test4()
{
    vector<int> a{1, 2, 3};
    vector<int> b{1, 4, 5 ,6};
    vector<int> c = merge<int>(a, b);
    print(a, b, c);
}

void test5()
{
    vector<int> a{};
    vector<int> b{1, 4, 5 ,6};
    vector<int> c = merge<int>(a, b);
    print(a, b, c);
}

void test6()
{
    vector<int> a{1, 4, 5 ,6};
    vector<int> b{};
    vector<int> c = merge<int>(a, b);
    print(a, b, c);
}

int main()
{
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    return 0;
}