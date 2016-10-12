# g++ -std=c++11 client/client.cpp tcp/tcpacceptor.cpp tcp/tcpstream.cpp tcp/tcpconnector.cpp -o client
# g++ -std=c++11 client/client_timeout.cpp tcp/tcpacceptor.cpp tcp/tcpstream.cpp tcp/tcpconnector.cpp -o client_timeout
# g++ -std=c++11 server/server.cpp tcp/tcpacceptor.cpp tcp/tcpstream.cpp tcp/tcpconnector.cpp -o server
# g++ -std=c++11 server/server_timeout.cpp tcp/tcpacceptor.cpp tcp/tcpstream.cpp tcp/tcpconnector.cpp -o server_timeout
# g++ -std=c++11 server/server_pause.cpp tcp/tcpacceptor.cpp tcp/tcpstream.cpp tcp/tcpconnector.cpp -o server_pause

g++ -std=c++11 test.cpp tcp/tcpsocket.cpp tcp/tcpacceptor.cpp tcp/tcpstream.cpp tcp/tcpconnector.cpp -o server_raft_leader
# ./server_raft_leader.exe localhost 3535 2 replicas_leader.bin log_leader.bin 1

g++ -std=c++11 test.cpp tcp/tcpsocket.cpp tcp/tcpacceptor.cpp tcp/tcpstream.cpp tcp/tcpconnector.cpp -o server_raft_follower
# ./server_raft_follower.exe localhost 3536 0 replicas_follower.bin log_follower.bin 1

g++ -std=c++11 client/client_raft.cpp tcp/tcpsocket.cpp tcp/tcpacceptor.cpp tcp/tcpstream.cpp tcp/tcpconnector.cpp -o client_raft
# ./client_raft.exe 127.0.0.1 3535