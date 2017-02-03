#include "huffman.h"
#include "bitvector.h"
#include <iostream>
#include <fstream>
#include <string>

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
        auto encoded = h.encode("aaaaaaaaaaaaaaabbbbbbbccccccddddddeeeee");
        auto decoded = h.decode(encoded);
        cout << encoded << endl
             << decoded << " size: " << decoded.size() * 8 << endl << endl;
    }

    {
        char buffer_in[] = {'a', 'a', '\0', 'b', 'c', 'a', 'b', '\0', '\0', 'a', 'a', 'b', 'a', 'a', 'a', 'b', 'c', 'a', 'b', '\0', '\0', 'a', 'a', 'b', 'a', 'a', 'a'};
        char buffer_out[1024];
        auto encoded = h.encode(buffer_in, 15);
        auto decodedSize = h.decode(encoded, buffer_out, 100);
        cout << encoded << endl;
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
        auto encoded = h.encode(buffer_in, 15);
        auto decodedSize = h.decode(encoded, buffer_out, 5);
        cout << encoded << endl;
        for(auto i = 0; i < decodedSize; i++)
        {
            if (buffer_out[i] == '\0')
                cout << "\\0";
            cout << buffer_out[i];
        }
        cout << " size: " << decodedSize * 8 << endl << endl;
    }

    {
        string input("aaaaaaaaaaaaaaabbbbbbbccccccddddddeeeee");
        auto encoded = h.encode(input);
        auto table  = h.get_translation_table();
        for(auto p: table)
        {
            cout << p.first << ": " << p.second << endl;
        }

        cout << encoded << endl
             << input << " size: " << input.size() * 8 << endl;

        huffman other;
        other.set_translation_table(table);
        auto decoded = other.decode(encoded);
        cout << decoded << " size: " << decoded.size() * 8 << endl << endl;
    }

    {
        string input("aaaaaaaaaaaaaaabbbbbbbccccccddddddeeeee");
        auto encoded = h.encode(input);

        ofstream fout("table.txt");
        h.store_translateion_table_to_ostream(fout);
        fout.close();
        auto table = h.get_translation_table();

        cout << "table stored to table.txt" << endl;
        for(auto p: table)
        {
            cout << p.first << " " << p.second << endl;
        }
        cout << endl;

        huffman other;
        ifstream fin("table.txt");
        other.load_translateion_table_from_istream(fin);
        fin.close();
             table = other.get_translation_table();

        cout << "table loaded from table.txt" << endl;
        for(auto p: table)
        {
            cout << p.first << " " << p.second << endl;
        }
        cout << endl;

        cout << encoded << endl
             << "initial to encode:  "
             << input << " size: " << input.size() * 8 << endl;

        auto decoded = other.decode(encoded);
        cout << "decoded with other: "
             << decoded << " size: " << decoded.size() * 8 << endl;
    }

    return 0;
}

/*

$ g++ -std=c++11 huffman_tree_node.cpp bitvector.cpp huffman.cpp test.cpp -o o && ./o.exe
01011110100111000000111111100001110 size: 35
adssadasdaaaaasssdaaasd size: 184

000000000000000111111111111111111111110110110110110110101101101101101101100100100100100 size: 87
aaaaaaaaaaaaaaabbbbbbbccccccddddddeeeee size: 312

11011000101000110111100111 size: 26
aa\0bcab\0\0aabaaa size: 120

11011000101000110111100111 size: 26
aa\0bc size: 40

d: 101 size: 3
e: 100 size: 3
a: 0 size: 1
b: 111 size: 3
c: 110 size: 3
000000000000000111111111111111111111110110110110110110101101101101101101100100100100100 size: 87
aaaaaaaaaaaaaaabbbbbbbccccccddddddeeeee size: 312
aaaaaaaaaaaaaaabbbbbbbccccccddddddeeeee size: 312

table stored to table.txt
d 101 size: 3
e 100 size: 3
a 0 size: 1
b 111 size: 3
c 110 size: 3

table loaded from table.txt
d 101 size: 3
e 100 size: 3
a 0 size: 1
b 111 size: 3
c 110 size: 3

000000000000000111111111111111111111110110110110110110101101101101101101100100100100100 size: 87
initial to encode:  aaaaaaaaaaaaaaabbbbbbbccccccddddddeeeee size: 312
decoded with other: aaaaaaaaaaaaaaabbbbbbbccccccddddddeeeee size: 312

*/