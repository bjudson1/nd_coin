#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>

using namespace std;

bool sendCoin(int, string, string, int);

int main(int argc, char *argv[]){
	int argind = 1;
    int client;
    int portNum = 8000; // NOTE that the port number is same for both client and server
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    char* ip = "127.0.0.1";
    string user,reciever;
    int coin;

    struct sockaddr_in server_addr;

    //get port number
    if(argc > 1)
    	portNum = atoi(argv[argind++]);


	//Get User
	cout<<"Username: ";
	cin>>user;

    //create socket
    if((client = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    	cout << "\nError establishing socket..." << endl;
        exit(1);
    }

    cout<<"=> Socket client has been created..." << endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);


    //connect
    if(connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        cout << "=> Connection to the server port number: " << portNum << endl;

    cout << "=> Awaiting confirmation from the server..." << endl; //line 40
    recv(client, buffer, bufsize, 0);
    cout << "=> Connection confirmed, you are good to go...";
    cout << "\n\n=> Enter # to end the connection\n" << endl;

    // Once it reaches here, the client can send a message first.
    do{
    	int choice;
    	string message;

    	cout<<"\n------MENU------\n";
    	cout<<"1:	Get Balance\n";
    	cout<<"2:	Send Coin\n";
    	cout<<"3:	Quit\n";
    	cin>>choice;

    	switch(choice){
    		case 1:
    			cout<<"Requesting Blance...\n";

    			message = "1 ";
    			message += user;

    			send(client,message.c_str(),sizeof(message),0);

    			recv(client, buffer, bufsize, 0);
    			cout<<"Balance: "<<buffer<<endl;
    		break;

    		case 2:
    			cout<<"How many coins would you like to send?";
    			cin>>coin;

    			cout<<"Who would you like to send to?";
    			cin>>reciever;

    			if(sendCoin(client,user,reciever,coin))
    				cout<<"Coins successfully sent\n";
    			else
    				cout<<"Coins failed to send\n";
    		break;

    		case 3:
    			isExit = true;
    		break;
    	}
    }while(!isExit);

    //close
    cout << "\n=> Connection terminated.\nGoodbye...\n";

    close(client);
    return 0;
}

bool sendCoin(int client, string sender, string reciever, int coin){
	string message =  "",coin_str = "";
	int bufsize = 1024;
    char buffer[bufsize];

    message += "2 ";
	message += sender;
	message += " ";
	message += reciever;
	message += " ";
	message += to_string(coin);
	message += "*";
	
	//send message to server
	send(client,message.c_str(),sizeof(message),0);

	//get return message from server
	recv(client, buffer, bufsize, 0);

	if(strcmp(buffer,"s") == 0)
		return true;

	return false;
}



