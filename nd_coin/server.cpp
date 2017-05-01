#include "ledger.h"
#include "bank.h"



#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

//#include <curlpp-0.8.1/include/curlpp/cURLpp.hpp>
//#include <curlpp-0.8.1/include/curlpp/Easy.hpp>
//#include <curlpp-0.8.1/include/curlpp/Options.hpp>

using namespace std;

bool makeTransfer(string,string,int);
ledger messageLedger = ledger();


bank the_bank;

struct coin{
	int serial_number;
};

int main(){

	int client,server;
	int portNum = 1500;
	int bufsize=1024;
	char buffer[bufsize];
	bool isExit = false;

	struct sockaddr_in server_addr;
    socklen_t size;

    //create socket
    if((client = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }

    cout << "\n=> Socket server has been created..." << endl;

    //set addr info
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);

    //bind socket
    if((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0){
        cout << "=> Error binding connection, the socket has already been established..." << endl;
        return -1;
    }
    size = sizeof(server_addr);


    //listen
    cout << "=> Looking for clients..." << endl;
    listen(client, 1);


    int clientCount = 1;
    server = accept(client,(struct sockaddr *)&server_addr,&size);

    // first check if it is valid or not
    if (server < 0) 
        cout << "=> Error on accepting..." << endl;

    //while connection to client
    while (server > 0){
        strcpy(buffer, "=> Server connected...\n");
        send(server, buffer, bufsize, 0);
        cout << "=> Connected with the client #" << clientCount << ", you are good to go..." << endl;
        cout << "\n=> Enter # to end the connection\n" << endl;

        cout << "Client: ";
        do {
            recv(server, buffer, bufsize, 0);
            cout << buffer << " ";
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != '*');

        do {
            cout << "\nServer: ";
            do {
                cin >> buffer;
                send(server, buffer, bufsize, 0);
                if (*buffer == '#') {
                    send(server, buffer, bufsize, 0);
                    *buffer = '*';
                    isExit = true;
                }
            } while (*buffer != '*');

            cout << "Client: ";
            do {
                recv(server, buffer, bufsize, 0);
                cout << buffer << " ";
                if (*buffer == '#') {
                    *buffer == '*';
                    isExit = true;
                }
            } while (*buffer != '*');
        } while (!isExit);


	//close
        // inet_ntoa converts packet data to IP, which was taken from client
        cout << "\n\n=> Connection terminated with IP " << inet_ntoa(server_addr.sin_addr);
        close(server);
        cout << "\nGoodbye..." << endl;
        isExit = false;
        exit(1);
    }

    close(client);
    return 0;

/*

	cout<<"Bob:"<<the_bank.getBalance("bob")<<endl;
	cout<<"cindy:"<<the_bank.getBalance("cindy")<<endl;

	//give bob coin
	the_bank.giveCoin("bob",2);


	cout<<"Bob:"<<the_bank.getBalance("bob")<<endl;
	cout<<"cindy:"<<the_bank.getBalance("cindy")<<endl;

	//transfer coin 
	if(makeTransfer("bob","cindy",2))
		cout<<"successful transfer\n";
	else{
		cout<<"transfer failed\n";
	}

	cout<<"Bob:"<<the_bank.getBalance("bob")<<endl;
	cout<<"cindy:"<<the_bank.getBalance("cindy")<<endl;

	return 0;

*/
}



bool makeTransfer(string sender,string reciever,int coin){
	
	//transfer
	if(the_bank.takeCoin(sender,coin)){
		the_bank.giveCoin(reciever,coin);
			
		//update ledger
		string message = sender + " sent " + reciever + " coin\n";
		messageLedger.putMessage(message);
		return true;
	}

	return false;
}
