//Brenden Judson
//DJ Chao
//client.cpp

#include "ledger.h"

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
#include <fstream>

#include "include/cryptopp/rsa.h"
#include "include/cryptopp/cryptlib.h"
#include "include/cryptopp/osrng.h"
#include "include/cryptopp/base32.h"
#include "include/cryptopp/files.h"
#include "include/cryptopp/filters.h"
#include "include/cryptopp/pssr.h"

using namespace std;

//----------------------------Prototype-----------------------------

void create_keys();
bool sendCoin(int, string, string, int,CryptoPP::RSA::PrivateKey,CryptoPP::RSA::PublicKey);
void usage();


//----------------------------GLOBAL-----------------------------
CryptoPP::AutoSeededRandomPool rng;

//ledger messageLedger = ledger();

int main(int argc, char *argv[]){
	int argind = 1;
    int client;
    int port = 8000; // NOTE that the port number is same for both client and server
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    char* ip = "127.0.0.1";
    string user,reciever;
    int coin;
    struct sockaddr_in server_addr;
    int length;

//---------------Parse Command Line-----------------------------
    char *arg;

    while(argind < argc){   
        arg = argv[argind++];

        if(strcmp(arg,"-h") == 0){
            usage();
            return 0;
        }

        else if(strcmp(arg,"-p") == 0){
            port =atoi(argv[argind++]);
        }

        else {
            usage();
            return -1;
        }
    }

	//Get User
	cout<<"Username: ";
	cin>>user;
//----------------------Create Key Pair--------------------------------------------
    CryptoPP::InvertibleRSAFunction params;
    params.GenerateRandomWithKeySize(rng, 3072);

    CryptoPP::RSA::PrivateKey privateKey(params);
    
    CryptoPP::RSA::PublicKey publicKey(params); 

//---------------Setup Client and Connect-----------------------------

    //create socket
    if((client = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    	cout << "\nError establishing socket..." << endl;
        exit(1);
    }

    cout<<"=> Socket client has been created..." << endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);


    //connect
    if(connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        cout << "=> Connection to the server port number: " << port << endl;

    cout << "=> Awaiting confirmation from the server..." << endl; //line 40
    recv(client, buffer, bufsize, 0);
    cout << "=> Connection confirmed, you are good to go...";
    cout << "\n\n=> Enter # to end the connection\n" << endl;

//---------------Execute-----------------------------

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
                cout<<"Message sent\n";

                //get response
    			length = recv(client, buffer, bufsize, 0);
                send(client,message.c_str(),sizeof(message),0);
                length = recv(client, buffer, bufsize, 0);
                cout<<"Message recieved\n";
                
                cout<<"Balance: ";
                for(int i=0 ;i< length;i++)
                    cout<<buffer[i];
                cout<<endl;
    		break;

    		case 2:
    			cout<<"How many coins would you like to send? ";
    			cin>>coin;

    			cout<<"Who would you like to send to? ";
    			cin>>reciever;

    			if(sendCoin(client,user,reciever,coin,privateKey,publicKey))
    				cout<<"Coins successfully sent\n";
    			else
    				cout<<"Coins failed to send\n";
    		break;

    		case 3:
                message = "3 ";
                send(client,message.c_str(),sizeof(message),0);
    			isExit = true;
    		break;
    	}
    }while(!isExit);

    //close
    cout << "\n=> Connection terminated.\nGoodbye...\n";

    close(client);
    return 0;
}



bool sendCoin(int client, string sender, string reciever, int coin,CryptoPP::RSA::PrivateKey privateKey,CryptoPP::RSA::PublicKey publicKey){
	string message =  "",output = "",coin_str = "",signature;
	int bufsize = 1024;
    char buffer[bufsize];

    message += "2 ";
	message += sender;
	message += " ";
	message += reciever;
	message += " ";
	message += to_string(coin);
	message += "*";

	

	CryptoPP::RSASSA_PKCS1v15_SHA_Signer signer(privateKey);

	CryptoPP::StringSource ss1(message, true, 
	    new CryptoPP::SignerFilter(rng, signer,
	        new CryptoPP::StringSink(signature)
	   ) // SignerFilter
	); // StringSource

    //send message with sig
	message += signature;
	send(client,message.c_str(),message.size(),0);
	cout<<"Message sent\n";

	//get return message from server
	recv(client, buffer, bufsize, 0);
    cout<<"Message Recieved\n";

	//save pub key to byte queu
	CryptoPP::ByteQueue queue;
    publicKey.Save(queue);	


    /*string filename = "yo.bin";
    CryptoPP::FileSink file(filename.c_str());
    queue.CopyTo(file);
    file.MessageEnd();*/

    //send public key
    byte buf[1024];
    fill(buf, buf +1024, 0);
    
    size_t bytes = queue.Get(buf,1024);
    int length = 1;

    while(bytes > 0){
        length = send(client,buf,bytes,0);
        cout<<"Sending Public Key...\tbytes: "<<bytes<<endl;
        bytes = queue.Get(buf,1024);
    }

    //send end message
    fill(buffer, buffer +1024, 0);
    buffer[0] = 'E';
    buffer[1] = 'O';
    buffer[2] = 'F';
    send(client,buffer,1024,0);
    cout<<"Sent Public Key\n";

	//get return message from server
	length = recv(client, buffer, bufsize, 0);
    message = "";
    for(int i=0;i<length;i++)  
        message+=buffer[i];

	if(strcmp(message.c_str(),"s") == 0)
		return true;

	return false;
}

void usage(){


}


