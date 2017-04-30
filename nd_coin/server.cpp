#include "ledger.h"
#include "bank.h"

using namespace std;

bool makeTransfer(string,string,int);

bank the_bank;

int main(){
	ledger messageLedger = ledger();

	cout<<"Bob:"<<the_bank.getBalance("bob")<<endl;
	cout<<"cindy:"<<the_bank.getBalance("cindy")<<endl;

	//give bob coin
	the_bank.setBalance("bob",1);


	cout<<"Bob:"<<the_bank.getBalance("bob")<<endl;
	cout<<"cindy:"<<the_bank.getBalance("cindy")<<endl;

	//transfer coin 
	makeTransfer("bob","cindy",1);

	cout<<"Bob:"<<the_bank.getBalance("bob")<<endl;
	cout<<"cindy:"<<the_bank.getBalance("cindy")<<endl;

	return 0;
}

bool makeTransfer(string sender,string reciever,int amount){
	//check to see if they have large enough balance
	if(the_bank.getBalance(sender) >= amount){
		//transfer
		int newBalance = the_bank.getBalance(sender) - amount;
		the_bank.setBalance(sender,newBalance);


		newBalance = the_bank.getBalance(reciever) + amount;
		the_bank.setBalance(reciever,newBalance);

		return true;
	}

	return false;
}
