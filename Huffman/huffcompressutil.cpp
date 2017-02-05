#include "huffman.h"
#include "bitvector.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void show_help()
{
    std::cout << "usege: util (--encode|--decode) infilename outfilename" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        show_help();
        return 0;
    }
    string method(argv[1]);
    string infilename(argv[2]);
    string outfilename(argv[3]);

    if (method != "--encode" && method != "--decode")
    {
        show_help();
        return 0;
    }

    if (method == "--encode")
    {
        ifstream fin(infilename.c_str());
        if(fin)
        {
            // get length of file:
            fin.seekg (0, fin.end);
            int length = fin.tellg();
            fin.seekg (0, fin.beg);

            char * buffer = new char[length];
            fin.read(buffer, length);
            fin.close();

            huffman encoder;
            auto encoded = encoder.encode(buffer, length);
            delete[] buffer;

            ofstream fout(outfilename.c_str());
            encoder.store_translateion_table_to_ostream(fout);
            fout << encoded;
            fout.close();
        }
        fin.close();
        return 0;
    }
    if (method == "--decode")
    {
        ifstream fin(infilename.c_str());
        if(fin)
        {
            huffman decoder;
            decoder.load_translateion_table_from_istream(fin);
            bitvector data;
            fin >> data;
            fin.close();

            size_t max_buff = 1024*1024*1024;
            char * outbuffer = new char[max_buff];
            auto decoded_length = decoder.decode(data, outbuffer, max_buff);
            cout << "decoded length: " << decoded_length;

            ofstream fout(outfilename.c_str());
            fout.write(outbuffer, decoded_length);
            fout.close();

            delete[] outbuffer;
        }
        return 0;
    }
    return 0;
}
