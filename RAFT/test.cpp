#include "logger.h"
#include "logger_test.h"
#include "operation.h"
#include "operation_test.h"
#include "store.h"
#include "store_test.h"

int main()
{
	test::logger_test lt;
	lt.run();

	test::operation_test ot;
	ot.run();

	test::store_test st;
	st.run();

	return 0;
}