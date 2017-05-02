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
bool sendCoin(int, string, string, int);
void usage();


//----------------------------GLOBAL-----------------------------


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

void create_keys(){
	


	/*const CryptoPP::Integer& n = params.GetModulus();
	const CryptoPP::Integer& p = params.GetPrime1();
	const CryptoPP::Integer& q = params.GetPrime2();
	const CryptoPP::Integer& d = params.GetPrivateExponent();
	const CryptoPP::Integer& e = params.GetPublicExponent();


	// Dump
	cout << "RSA Parameters:" << endl;
	cout << " n: " << n << endl;
	cout << " p: " << p << endl;
	cout << " q: " << q << endl;
	cout << " d: " << d << endl;
	cout << " e: " << e << endl;
	cout << endl;*/
}


bool sendCoin(int client, string sender, string reciever, int coin){
	

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

	//create public and private key pair
	CryptoPP::AutoSeededRandomPool rng;

	CryptoPP::InvertibleRSAFunction params;
	params.GenerateRandomWithKeySize(rng, 3072);

	CryptoPP::RSA::PrivateKey privateKey(params);
	CryptoPP::RSA::PublicKey publicKey(params);	

	CryptoPP::RSASSA_PKCS1v15_SHA_Signer signer(privateKey);

	CryptoPP::StringSource ss1(message, true, 
	    new CryptoPP::SignerFilter(rng, signer,
	        new CryptoPP::StringSink(signature)
	   ) // SignerFilter
	); // StringSource

	message += signature;
	//message += "EOFEOF";

	//cout<<"Message: "<<message<<endl<<endl;
	//cout<<"Message C_str: "<<message.c_str()<<endl;

	send(client,message.c_str(),message.size(),0);
	cout<<"Message sent\n";

	//get return message from server
	recv(client, buffer, bufsize, 0);


	//send public key
	CryptoPP::ByteQueue queue;
	privateKey.DEREncodePrivateKey(queue);
	cout<<"size: "<<sizeof(queue)<<endl;
	send(client,&queue,sizeof(queue),0);





	//get return message from server
	recv(client, buffer, bufsize, 0);

	if(strcmp(buffer,"s") == 0)
		return true;

	return false;
}

void usage(){


}


