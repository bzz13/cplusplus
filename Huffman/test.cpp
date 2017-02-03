#include "Huffman.h"
#include "bitvector.h"
#include <iostream>

using namespace std;

int main()
{

    huffman h;

    {
        auto encoded = h.encode("adssadasdaaaaasssdaaasd");
        auto decoded = h.decode(encoded);
        cout << encoded << endl << decoded << " size: " << decoded.size() * 8 << endl << endl;
    }

    {
        auto enoded = h.encode("aaaaaaaaaaaaaaabbbbbbbccccccddddddeeeee");
        auto decoded = h.decode(enoded);
        cout << enoded << endl
             << decoded << " size: " << decoded.size() * 8 << endl << endl;
    }

    {
        char buffer_in[] = {'a', 'a', '\0', 'b', 'c', 'a', 'b', '\0', '\0', 'a', 'a', 'b', 'a', 'a', 'a', 'b', 'c', 'a', 'b', '\0', '\0', 'a', 'a', 'b', 'a', 'a', 'a'};
        char buffer_out[1024];
        auto enoded = h.encode(buffer_in, 15);
        auto decodedSize = h.decode(enoded, buffer_out, 100);
        cout << enoded << endl;
        for(auto i = 0; i < decodedSize; i++)
        {
            if (buffer_out[i] == '\0')
                cout << "\\0";
            cout << buffer_out[i];
        }
        cout << " size: " << decodedSize * 8 << endl << endl;
    }

    {
        char buffer_in[] = {'a', 'a', '\0', 'b', 'c', 'a', 'b', '\0', '\0', 'a', 'a', 'b', 'a', 'a', 'a', 'b', 'c', 'a', 'b', '\0', '\0', 'a', 'a', 'b', 'a', 'a', 'a'};
        char buffer_out[1024];
        auto enoded = h.encode(buffer_in, 15);
        auto decodedSize = h.decode(enoded, buffer_out, 5);
        cout << enoded << endl;
        for(auto i = 0; i < decodedSize; i++)
        {
            if (buffer_out[i] == '\0')
                cout << "\\0";
            cout << buffer_out[i];
        }
        cout << " size: " << decodedSize * 8 << endl << endl;
    }

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

b: 00 size: 2
c: 010 size: 3
\0: 011 size: 3
a: 1 size: 1
11011000101000110111100111 size: 26
aa\0bcab\0\0aabaaa size: 120

b: 00 size: 2
c: 010 size: 3
\0: 011 size: 3
a: 1 size: 1
11011000101000110111100111 size: 26
aa\0bc size: 40

*/