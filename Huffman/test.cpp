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