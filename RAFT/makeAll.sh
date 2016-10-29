rm -f server_raft.exe client_raft.exe
rm -f r1/server_raft.exe r2/server_raft.exe r3/server_raft.exe

# g++ -std=c++11 client/client.cpp tcp/tcpacceptor.cpp tcp/tcpstream.cpp tcp/tcpconnector.cpp -o client
# g++ -std=c++11 client/client_timeout.cpp tcp/tcpacceptor.cpp tcp/tcpstream.cpp tcp/tcpconnector.cpp -o client_timeout
# g++ -std=c++11 server/server.cpp tcp/tcpacceptor.cpp tcp/tcpstream.cpp tcp/tcpconnector.cpp -o server
# g++ -std=c++11 server/server_timeout.cpp tcp/tcpacceptor.cpp tcp/tcpstream.cpp tcp/tcpconnector.cpp -o server_timeout
# g++ -std=c++11 server/server_pause.cpp tcp/tcpacceptor.cpp tcp/tcpstream.cpp tcp/tcpconnector.cpp -o server_pause

g++ -std=c++11 test.cpp -o server_raft
# ./server_raft.exe localhost 3536 replicas.bin log.bin 0

g++ -std=c++11 client/client_raft.cpp -o client_raft
# ./client_raft.exe 127.0.0.1 3535

cp server_raft.exe r1/server_raft.exe
cp server_raft.exe r2/server_raft.exe
cp server_raft.exe r3/server_raft.exe