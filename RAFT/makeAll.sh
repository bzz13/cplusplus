# g++ -std=c++11 client/client.cpp tcp/tcpacceptor.cpp tcp/tcpstream.cpp tcp/tcpconnector.cpp -o client
# g++ -std=c++11 client/client_timeout.cpp tcp/tcpacceptor.cpp tcp/tcpstream.cpp tcp/tcpconnector.cpp -o client_timeout
# g++ -std=c++11 server/server.cpp tcp/tcpacceptor.cpp tcp/tcpstream.cpp tcp/tcpconnector.cpp -o server
# g++ -std=c++11 server/server_timeout.cpp tcp/tcpacceptor.cpp tcp/tcpstream.cpp tcp/tcpconnector.cpp -o server_timeout
# g++ -std=c++11 server/server_pause.cpp tcp/tcpacceptor.cpp tcp/tcpstream.cpp tcp/tcpconnector.cpp -o server_pause

g++ -std=c++11 test.cpp tcp/tcpacceptor.cpp tcp/tcpstream.cpp tcp/tcpconnector.cpp -o server_raft
g++ -std=c++11 client/client_raft.cpp tcp/tcpacceptor.cpp tcp/tcpstream.cpp tcp/tcpconnector.cpp -o client_raft