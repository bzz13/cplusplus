#include "Huffman.h"
#include "bitvector.h"
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    // {
    //     bitvector b;
    //     cin >> b;
    //     cout << b << endl << endl;
    // }
    {
        bitvector b(10, false);
        cout << b << endl;
        b.push_back(true);
        cout << b << endl << endl;
    }
    {
        bitvector b(29, true);
        cout << b << endl;
        b.push_back(true);
        cout << b << endl << endl;
    }
    {
        bitvector b(129, true);
        cout << b << endl << endl;
    }
    {
        bitvector b;
        b.push_back(false);
        cout << b[0] << endl;
        b[0] = true;
        cout << b[0] << endl;
        b[0] = true;
        cout << b[0] << endl;
        b[0] = false;
        cout << b[0] << endl << endl;
    }
    {
        auto str = "1001";
        cout << str << endl;
        bitvector b(str);
        cout << b << endl << endl;
    }
    {
        auto str = "11001101";
        cout << str << endl;
        bitvector b(str);
        cout << b << endl << endl;
    }
    {
        auto str = "110011010010001";
        cout << str << endl;
        bitvector b(str);
        cout << b << endl << endl;
    }
    {
        bitvector b;

        b.push_back(true);
        cout << b << " " << b[0] << endl;
        b.push_back(false);
        cout << b << " " << b[1] << endl;
        b.push_back(true);
        cout << b << " " << b[2] << endl;
        b.push_back(true);
        cout << b << " " << b[3] << endl;
        b.push_back(false);
        cout << b << " " << b[4] << endl;
        b.push_back(true);
        cout << b << " " << b[5] << endl;
        b.push_back(true);
        cout << b << " " << b[6] << endl;
        b.push_back(true);
        cout << b << " " << b[7] << endl;
        b.push_back(false);
        cout << b << " " << b[8] << endl;
        b.push_back(true);
        cout << b << " " << b[9] << endl << endl;
    }
    {
        cout << "110011010010001" << endl;
        bitvector b("110011010010001");
        for(auto bit: b)
        {
            cout << bit;
            bit = !bit;
        }
        cout << endl << b << endl << endl;
    }
    {
        bitvector a("110011010010001");
        cout << a << endl;
        bitvector b("1001");
        cout << b << endl;
        a.push_back(b);
        cout << a << endl << endl;
    }
    {
        bitvector a("1100110110111011");
        ofstream fout("out.txt");
        fout << a;
        fout.close();

        bitvector b;
        ifstream fin("out.txt");
        fin >> b;

        cout << a << endl
             << b << endl << endl;
    }
    return 0;
}