#include "huffman.h"
#include "bitvector.h"
#include <iostream>
#include <fstream>

using namespace std;

int main()
{

    ifstream fin_data("part1.mov", std::ifstream::binary);
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

        ofstream fout_table("part1.mov.tbl");
        encoder.store_translateion_table_to_ostream(fout_table);
        fout_table.close();

        ofstream fout_data("part1.mov.data");
        fout_data << encoded;
        fout_data.close();
    }

    ifstream fin_enc_data("part1.mov.data");
    bitvector data;
    fin_enc_data >> data;
    fin_enc_data.close();

    huffman decoder;
    ifstream fin_enc_table("part1.mov.tbl");
    decoder.load_translateion_table_from_istream(fin_enc_table);
    fin_enc_table.close();

    size_t max_buff = 1024*1024*1024;
    char * outbuffer = new char[max_buff];
    auto decoded_length = decoder.decode(data, outbuffer, max_buff);
    cout << "decoded length: " << decoded_length;

    ofstream fout_data("part1.copy.mov", std::ofstream::binary);
    fout_data.write(outbuffer, decoded_length);
    fout_data.close();

    delete[] outbuffer;

    return 0;
}
