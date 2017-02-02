#include "Huffman.h"
#include "bitvector.h"
#include <iostream>

using namespace std;

int main()
{

    huffman h;

    auto en1 = h.encode("adssadasdaaaaasssdaaasd");
    auto de1 = h.decode(en1);
    cout << en1 << endl << de1 << " size: " << de1.size() * 8 << endl << endl;

    auto en2 = h.encode("aaaaaaaaaaaaaaabbbbbbbccccccddddddeeeee");
    auto de2 = h.decode(en2);
    cout << en2 << endl
         << de2 << " size: " << de2.size() * 8 << endl;
    return 0;
}

/*

s: 11 size: 2
d: 10 size: 2
a: 0 size: 1
01011110100111000000111111100001110 size: 35
adssadasdaaaaasssdaaasd size: 184

e: 100 size: 3
a: 0 size: 1
b: 111 size: 3
c: 110 size: 3
d: 101 size: 3
000000000000000111111111111111111111110110110110110110101101101101101101100100100100100 size: 87
aaaaaaaaaaaaaaabbbbbbbccccccddddddeeeee size: 312

*/