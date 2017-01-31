#include "Huffman.h"
#include "bitvector.h"
#include <iostream>

using namespace std;

int main()
{
    {
        bitvector b;
        b.push(false);
        cout << b[0] << endl;
        b[0] = true;
        cout << b[0] << endl;
        b[0] = true;
        cout << b[0] << endl;
        b[0] = false;
        cout << b[0] << endl;
    }
    {
        bitvector b;

        b.push(true);
        cout << b << " " << b[0] << endl;
        b.push(false);
        cout << b << " " << b[1] << endl;
        b.push(true);
        cout << b << " " << b[2] << endl;
        b.push(true);
        cout << b << " " << b[3] << endl;
        b.push(false);
        cout << b << " " << b[4] << endl;
        b.push(true);
        cout << b << " " << b[5] << endl;
        b.push(true);
        cout << b << " " << b[6] << endl;
        b.push(true);
        cout << b << " " << b[7] << endl;
        b.push(false);
        cout << b << " " << b[8] << endl;
        b.push(true);
        cout << b << " " << b[9] << endl;
    }

    // huffman h;
    // auto en1 = h.encode("adssadasdaaaaasssdaaasd");
    // cout << en1 << endl;
    // cout << h.decode(en1) << endl;
    // auto en2 = h.encode("aaaaaaaaaaaaaaabbbbbbbccccccddddddeeeee");
    // cout << en2 << endl;
    // cout << h.decode(en2) << endl;
    return 0;
}