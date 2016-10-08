#include "logger.h"
#include <iostream>
#include <vector>
#include <cstdlib>

namespace test
{
	using namespace std;

	struct test_logger_struct
	{
		int idx;
		int key;
		int val;
	};

	typedef logger<test_logger_struct> testlogger;

	class logger_test
	{
		void printLogger(testlogger& l)
		{
			for(int i = 0; i < l.maxIndex(); ++i)
			{
				test_logger_struct t = l.get(i);
				cout << t.idx << " " << t.key << " " << t.val << endl;
			}
		}
	public:
		void run()
		{
			{
				cout << "generating..." << endl;
				testlogger l("tmp.bin");
				vector<test_logger_struct> v;
				int maxsize = rand() % 20;
				for(int i = 0; i < maxsize; ++i)
				{
					test_logger_struct t = { rand() % 1000, rand() % 1000, rand() % 1000 };
					cout << t.idx << " " << t.key << " " << t.val << endl;
					l.set(i, t);
				}
				cout << endl;
			}
			{
				cout << "reading..." << endl;
				testlogger l("tmp.bin", true);
				printLogger(l);
				cout << endl;
			}
			{
				cout << "modifing..." << endl;
				testlogger l("tmp.bin", true);
				l.set(3, test_logger_struct { 0, 0 , 0 });
				printLogger(l);
				cout << endl;
			}
			{
				cout << "appending..." << endl;
				testlogger l("tmp.bin", true);
				l.set(l.maxIndex(), test_logger_struct { 10, 10, 10 });
				l.app(test_logger_struct { 20, 20, 20 });
				printLogger(l);
				cout << endl;
			}
			{
				cout << "write to out of range..." << endl;
				testlogger l("tmp.bin", true);
				try
				{
					l.set(l.maxIndex()*2, test_logger_struct { 30, 30, 30 });
				}
				catch(out_of_range &e)
				{
					cout << "catch " << e.what() << endl;
				}
			}
		}
	};
}