//server.cpp
//Brenden Judson
//DJ Chao

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
#include <vector>

#include "include/cryptopp/rsa.h"
#include "include/cryptopp/cryptlib.h"
#include "include/cryptopp/osrng.h"
#include "include/cryptopp/base32.h"
#include "include/cryptopp/files.h"
#include "include/cryptopp/filters.h"
#include "include/cryptopp/pssr.h"

using namespace std;

//---------------Prototypes-----------------------------

bool makeTransfer(string,string,int);
void set_up_test();
void usage();
int socket_listen(int);

//---------------Globals-----------------------------

ledger messageLedger = ledger();


bank the_bank;

struct coin{
	int serial_number;
};

int main(int argc, char *argv[]){
	int client,server;
	int argind = 1;
	int port = 8000;
	int bufsize=1024;
	char buffer[bufsize];
	bool isExit = false;

	struct sockaddr_in server_addr;
    socklen_t size;

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

        else if(strcmp(arg,"-test") == 0){
            set_up_test();
        }

        else {
            usage();
            return -1;
        }
    }

//---------------Set Up Server + Begin accepting-----------------------------
    int clientCount = 1;
    int i =0;
    string message = "",sender ="",reciever="",coin="",signature="";
    char type;

    //listen on port
    if((server = socket_listen(port)) < 0){
        return -1;
    }

    do{
        //accept client
        cout << "Looking for clients..." << endl;
        client = accept(server,(struct sockaddr *)&server_addr,&size);

        // first check if it is valid or not
        if (client < 0) 
            cout << "=> Error on accepting..." << endl;
            
        //confirm conection
        strcpy(buffer, "Server connected...\n");
        send(client, buffer, bufsize, 0);
        cout<<"Connected with the client"<<clientCount<<endl;
        
        //get original message from client
        int length;

        cout<<"Waiting for Message...\n";

        length = recv(client, buffer, bufsize, 0);
     
        cout<<"Message Recieved\n";
        message.append(buffer, buffer + length);

        string output = "s";
        send(client, output.c_str(), output.size(), 0);

        //-------------Handle Message----------------------------------------
        //get message type
        type = message[i++];
        i++;

        switch(type){
            case '1':      //get balance message
                cout<<"Getting Balance\n";
                

                    //balance = the_bank.getBalance(sender);
                    //message = to_string(balance);
                    //cout<<message;
                //  send(server, message.c_str(), sizeof(message), 0);
            break;
      
            case '2':    //transfer coin
                cout<<"Transfer Request...\n";

                //parse message
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
                i++;

                signature.append(buffer + i, buffer + length); 
               

                byte buf[1024];
                fill(buf, buf + 1024, 0);
                CryptoPP::ByteQueue ByteQueue(0);

                //get first packet
                length = recv(client, buf, 1024, 0);
                cout<<"Recieving PublicKey...\tbytes: "<<length<<endl;
                

                int end=0;
                bool over = false;

                if((char)buf[0] == 'E' && (char)buf[1] == 'O' && (char)buf[2] == 'F')
                    cout<<"Nothing Recieved\n";

                while(end < 1024){
                        if(buf[end] == 'E' && buf[end+1] == 'O' && buf[end+2] == 'F'){
                            over = true;
                            break;
                        }
                        end++;
                }

                ByteQueue.Put2((byte *)buf, end, 0, false); 

                while(!over){
                    cout<<"Recieving PublicKey...\tbytyes: "<<length<<endl;
                    fill(buf, buf + 1024, 0);
                    length = recv(client, buf, 1024, 0);

                    end=0;
                    while(end < 1024){
                        if(buf[end] == 'E' && buf[end+1] == 'O' && buf[end+2] == 'F'){
                            over = true;
                            break;
                        }
                        end++;
                    }

                    ByteQueue.Put2((byte *)buf, end, 0, false); 

                    if(over)
                        break;
                }
                cout<<"Recieved Public Key\n";

 
                //load pub key from byte queue
                CryptoPP::RSA::PublicKey publicKey;
                publicKey.Load(ByteQueue); 

                //Verify and Recover
                bool result = false;
                CryptoPP::RSASSA_PKCS1v15_SHA_Verifier verifier(publicKey);
                CryptoPP::StringSource ss2(message, true,
                    new CryptoPP::SignatureVerificationFilter(
                        verifier, 
                        new CryptoPP::ArraySink(
                            (byte*)&result, sizeof(result)),
                            CryptoPP::SignatureVerificationFilter::PUT_RESULT | CryptoPP::SignatureVerificationFilter::SIGNATURE_AT_END
                    )
                ); // StringSource
                
                if(true != result){
                    cout<<"Failed to Verify signature on message"<<endl;
                }

                else{
                    cout<<"Verified signature on message"<<endl;

                    if(makeTransfer(sender,reciever,atoi(coin.c_str()))){
                        cout<<"Transfer accpeted\n";
                        message = "s";
                        send(client, message.c_str(), message.size(), 0);
                    }

                    else{
                        cout<<"Transfer denied\n";
                        message = "f";
                        send(client, message.c_str(), message.size(), 0);
                    }
                }
            break;
        };
        
        //-----------CLOSE ClIENT CONNECTION---------------------------------------

        cout<<"\nConnection terminated with Client IP: "<<inet_ntoa(server_addr.sin_addr)<<endl;
        close(client);
    }while(!isExit);
    
//-----------CLOSE SERVER---------------------------------------
    close(server);
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

	cout<<"TEST\n";
    cout<<"bjudson: 100 coins\n";
    cout<<"dchao: 100 coins\n";
}

int socket_listen(int port){
    int server;
    struct sockaddr_in server_addr;

    //create socket
    if((server = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        cout << "\nError establishing socket..." << endl;
        return -1;
    }

    //set addr info
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(port);

    //bind socket
    if((bind(server, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0){
        cout << "Error binding connection, the socket has already been established..." << endl;
        return -1;
    }
    //size = sizeof(server_addr);

    cout<<"Socket server has been created on port "<<port<<endl;
    
    //listen
    listen(server, 1);

    return server;
}

void usage(){
    cout<<"USAGE: ./server.cpp [FLAGS]\n";
}