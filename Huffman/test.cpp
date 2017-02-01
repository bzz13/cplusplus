#include "Huffman.h"
#include "bitvector.h"
#include <iostream>

using namespace std;

int main()
{

    huffman h;
    auto en1 = h.encode("adssadasdaaaaasssdaaasd");
    cout << en1 << endl;
    cout << h.decode(en1) << endl;
    auto en2 = h.encode("aaaaaaaaaaaaaaabbbbbbbccccccddddddeeeee");
    cout << en2 << endl;
    cout << h.decode(en2) << endl;
    return 0;
}