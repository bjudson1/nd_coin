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
void set_up_test();
ledger messageLedger = ledger();


bank the_bank;

struct coin{
	int serial_number;
};

int main(int argc, char *argv[]){
	int client,server;
	int argind = 1;
	int portNum = 8000;
	int bufsize=1024;
	char buffer[bufsize];
	bool isExit = false;

	struct sockaddr_in server_addr;
    socklen_t size;

    the_bank.giveCoin("bob",10);

    //get port number
    if(argc > 1)
    	portNum =atoi(argv[argind++]);

    set_up_test();

    //create socket
    if((client = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }

    cout<<"Socket server has been created on port "<<portNum<<endl;

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
        
    	//confirm conection
        strcpy(buffer, "=> Server connected...\n");
        send(server, buffer, bufsize, 0);
        
        cout << "=> Connected with the client #" << clientCount << ", you are good to go..." << endl;
        cout << "\n=> Enter # to end the connection\n" << endl;

        do{
        	string sender = "",reciever = "",coin = "";
        	int i=0;

        	recv(server, buffer, bufsize, 0);
        	
        	//parse message for type
        	char type = buffer[i++];
        	i++;

        	int balance;
        	string message;

        	//decide what to do
        	switch(type){
        		//get balance
        		case '1':
        			cout<<"Getting Balance\n";
        			cout<<buffer;

        			/*while(buffer[i] != ' '){
        				sender += buffer[i++];
        			}*/
        			cout<<"F";

        			//balance = the_bank.getBalance(sender);
        			//message = to_string(balance);
        			//cout<<message;
        		//	send(server, message.c_str(), sizeof(message), 0);
        		break;

        		//transfer coin
        		case '2':
        			cout<<"Transfer Request\n";

					while(buffer[i] != ' '){
        				sender += buffer[i++];
        			}
        			i++;
        			while(buffer[i] != ' '){
        				reciever += buffer[i++];
        			}
        			i++;
        			while(buffer[i] != '*'){
        				coin += buffer[i++];
        			}

					if(makeTransfer(sender,reciever,atoi(coin.c_str()))){
        				message = "s";

        				send(server, message.c_str(), sizeof(message), 0);

        				isExit = true;
        			}

        			else{
        				message = "f";

        				send(server, message.c_str(), sizeof(message), 0);
        			}
        		break;
        	};
        }while(!isExit);
   

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


void set_up_test(){
	for(int i=0;i<100;i++)
		the_bank.giveCoin("bjudson1",i);

	for(int j=101;j<200;j++)
		the_bank.giveCoin("dchao",j);

	cout<<"f";
}
