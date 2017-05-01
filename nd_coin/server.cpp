#include "ledger.h"
#include "bank.h"

#include "./crypto++/rsa.h"
#include "crypto++/osrng.h"
//#include "crypto++/base32.h"
#include "crypto++/files.h"


using namespace std;

bool makeTransfer(string,string,int);
ledger messageLedger = ledger();


bank the_bank;

struct coin{
	int serial_number;
};

int main(){


 CryptoPP::AutoSeededRandomPool rng;
 CryptoPP::InvertibleRSAFunction privkey;
 privkey.Initialize(rng, 1024);
	//srand (time(NULL));

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
