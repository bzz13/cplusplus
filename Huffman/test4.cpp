#include "huffman.h"
#include "bitvector.h"
#include <iostream>
#include <fstream>

using namespace std;

int main()
{

    ifstream fin_data("lt1.txt");
    if(fin_data)
    {
        // get length of file:
        fin_data.seekg (0, fin_data.end);
        int length = fin_data.tellg();
        fin_data.seekg (0, fin_data.beg);

        char * buffer = new char[length];
        fin_data.read(buffer, length);

        huffman encoder;
        auto encoded = encoder.encode(buffer, length);
        delete[] buffer;

        ofstream fout("lt1.txt.huf");
        encoder.store_translateion_table_to_ostream(fout);
        fout << encoded;
        fout.close();
    }

    huffman decoder;
    ifstream fin("lt1.txt.huf");
    decoder.load_translateion_table_from_istream(fin);
    bitvector data;
    fin >> data;
    fin.close();

    size_t max_buff = 1024*1024*1024;
    char * outbuffer = new char[max_buff];
    auto decoded_length = decoder.decode(data, outbuffer, max_buff);
    cout << "decoded length: " << decoded_length;

    ofstream fout_data("lt1.copy.txt", std::ofstream::binary);
    fout_data.write(outbuffer, decoded_length);
    fout_data.close();

    delete[] outbuffer;

    return 0;
}
