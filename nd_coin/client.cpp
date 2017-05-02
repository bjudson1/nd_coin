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


//----------------------------GLOBAL-----------------------------


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
	

	string message =  "",coin_str = "",signature;
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


	cout<<"Message: "<<message<<endl;

	CryptoPP::StringSource ss1(message, true, 
	    new CryptoPP::SignerFilter(rng, signer,
	        new CryptoPP::StringSink(signature)
	   ) // SignerFilter
	); // StringSource

	message += signature;
	message += "EOF";

	cout<<"Message: "<<sizeof(signature.c_str())<<endl;



/*
	// Verify and Recover
	CryptoPP::RSASSA_PKCS1v15_SHA_Verifier verifier(publicKey);

	CryptoPP::StringSource ss2(message+signature, true,
	    new CryptoPP::SignatureVerificationFilter(
	        verifier, NULL,
	        CryptoPP::SignatureVerificationFilter::THROW_EXCEPTION
	   ) // SignatureVerificationFilter
	); // StringSource

	cout << "Verified signature on message" << endl;*/

	
	/*CryptoPP::RSASS<PSSR, SHA1>::Signer signer;
	CryptoPP::RSASS<PSSR, SHA1>::Verifier verifier;
*/




	/*

	//Encrypt Message
	string signature;

	////////////////////////////////////////////////
	// Sign and Encode
	CryptoPP::RSASSA_PKCS1v15_SHA_Signer signer(CryptoPP::RSA::PrivateKey);


	new CryptoPP::SignerFilter(rng, signer);


	CryptoPP::StringSource ss1(message, true, 
	    new CryptoPP::SignerFilter(rng, signer,
	        new CryptoPP::StringSink(signature),false
	   ) // SignerFilter
	); // StringSource
*/



	//send message to server
	//send(client,message.c_str(),sizeof(message),0);
	send(client,signature.c_str(),sizeof(signature.c_str()),0);

	//get return message from server
	recv(client, buffer, bufsize, 0);

	if(strcmp(buffer,"s") == 0)
		return true;

	return false;
}



