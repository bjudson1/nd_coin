CMP = g++
CLASS1 = ledger
CLASS2 = bank
MAIN = server

all: run_server run_client


run_server: ledger.o bank.o server.o libcryptopp.a
	$(CMP) -L. libcryptopp.a ledger.o bank.o server.o -o run_server

ledger.o: ledger.cpp ledger.h
	$(CMP) -c ledger.cpp -o ledger.o

bank.o: bank.cpp bank.h
	$(CMP) -c bank.cpp -o bank.o

server.o: server.cpp bank.h ledger.h
	$(CMP) -c server.cpp -o server.o




run_client: client.o libcryptopp.a
	$(CMP) -L. libcryptopp.a client.o -o run_client

client.o: client.cpp
	$(CMP) -c -o client.o client.cpp 



clean:
	rm *.o
	rm run_server
	rm run_client
