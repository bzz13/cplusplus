#include "logger.h"
#include "logger_test.h"
#include "operation.h"
#include "operation_test.h"
#include "store.h"
#include "store_test.h"
#include "server/server_raft.h"
#include "replicas/replica.h"

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

	if (argc != 7)
	{
		std::cerr << "usage: server <string hosthame> <int port> <int status> <string replicaspath> <string logpath> <bool restore 1=true>" << std::endl;
		return 1;
	}

	replica self(argv[1], atoi(argv[2]));
	server_raft<int, int> raft(self, atoi(argv[3]), argv[4], argv[5], atoi(argv[6]) == 1);
	raft.start();

	return 0;
}