#include "bank.h"



bank::bank(){
	//set coin serial numbers

}

//add coin to account
void bank::giveCoin(string account_name,int serial_number){
	//if the account exists
	if(accounts.find(account_name) != accounts.end())
		accounts[account_name].serial_numbers.push_back(serial_number);

	else{
		//else create account
		account newAccount;
		newAccount.serial_numbers.push_back(serial_number);
		map<string,account>::iterator it = accounts.begin();
		accounts.insert(it,pair<string,account>(account_name,newAccount));
	}
}

//take coin from account
bool bank::takeCoin(string account_name,int serial_number){
	//if the account exists
	if(accounts.find(account_name) != accounts.end()){
		//search account for coin
		for(vector<int>::iterator it=accounts[account_name].serial_numbers.begin(); it!=accounts[account_name].serial_numbers.end(); ++it){
			//if found
			if(*it == serial_number){
				accounts[account_name].serial_numbers.erase(it);
				return true;
			}
		}
	}

	return false;
}

void bank::makeCoin(){
//	serial = rand() % 1000;

//	coins.push_back();
}
/*void bank::setBalance(string account_name,int amount){
	map<string,account>::iterator it = accounts.begin();

	accounts.erase(account_name);
	accounts.insert(it,pair<string,int>(account_name,amount));
}*/

int bank::getBalance(string name){
	return accounts[name].serial_numbers.size();
}
