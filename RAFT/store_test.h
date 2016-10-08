#include "store.h"
#include "operation.h"
#include "logger.h"
#include <iostream>

namespace test
{
	using namespace std;

	class store_test
	{
	public:
		void run()
		{
			{
				cout << endl << "store filling..." << endl;
				store<const char*, int> s("tmp.bin");
				s.set("x", 1);
				s.set("x", 2);
				s.set("y", 10);
				s.showStore();
			}
			{
				cout << endl << "store restoring..." << endl;
				store<const char*, int> s("tmp.bin", true);
				s.showStore();
			}
			{
				cout << endl << "store modifying..." << endl;
				store<const char*, int> s("tmp.bin", true);
				s.set("x", 100);
				s.del("y");
				s.showStore();
			}
		}
	};
}