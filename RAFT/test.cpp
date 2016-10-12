#include "logger.h"
#include "logger_test.h"
#include "operation.h"
#include "operation_test.h"
#include "store.h"
#include "store_test.h"
#include "server/server_raft.h"

#include <string.h>

int main(int argc, char** argv)
{
	// test::logger_test lt;
	// lt.run();

	// test::operation_test ot;
	// ot.run();

	// test::store_test st;
	// st.run();

	//port status replicas log restore(1)

	if (argc != 6)
	{
		std::cerr << "usage: server <int port> <int status> <string replicaspath> <string logpath> <bool restore 1=true>" << std::endl;
		return 1;
	}
	server_raft<int, int> raft(atoi(argv[1]), atoi(argv[2]), argv[3], argv[4], atoi(argv[5]) == 1);
	raft.start();

	return 0;
}