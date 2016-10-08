#include "operation.h"
#include <string>

namespace test
{
	using namespace std;

	class test_op_class
	{
		int a;
		int b;
	public:
		test_op_class(int aa, int bb): a(aa), b(bb) { };

		int getA() const { return a; }
		int getB() const { return b; }
	};

	typedef operation<std::string, test_op_class> testop;

	class operation_test
	{
		void print(const testop& a)
		{
			cout << a.index() 
				 << ": type = " << a.type() 
				 << ", key = " << a.key() 
				 << ", val = {" << a.val().getA() << ":" << a.val().getB() << "}" 
				 << endl;
		}
	public:
		void run()
		{
			{
				cout << endl << "testing operation..." << endl;
				testop a(opertaion_type::set, "x", test_op_class(123, 321));
				testop b(opertaion_type::del, "y", test_op_class(1,4));
				testop c(opertaion_type::set, "x", test_op_class(123, 0));
				print(a);
				print(b);
				print(c);
			}
		}
	};
}