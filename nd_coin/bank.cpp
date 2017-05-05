#include "bank.h"
#include <stdio.h>

bank::bank(){
	//set coin serial numbers
	accounts = map<string,account>();
	vector<int> coins = vector<int>();

}

//add coin to account
void bank::giveCoin(string account_name,int serial_number){
	//if the account exists
	if(accounts.find(account_name) != accounts.end()){
		accounts[account_name].serial_numbers.push_back(serial_number);
	}

	else{
		//else create account
		account newAccount;
		newAccount.serial_numbers.push_back(serial_number);
		map<string,account>::iterator it = accounts.begin();
		accounts.insert(it,pair<string,account>(account_name,newAccount));
	}
}


//take coin from account
int bank::takeCoin(string account_name){
	//if the account exists
	if(accounts.find(account_name) != accounts.end()){
		int num = accounts[account_name].serial_numbers.back();
		accounts[account_name].serial_numbers.pop_back();
		
		return num;
	}
	
	return -1;
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


int bank::getBalance(string name){
	for(map<string,account>::iterator it = accounts.begin();it != accounts.end(); ++it){
		if(strcmp(name.c_str(),it->first.c_str()) == 0){
			return it->second.serial_numbers.size();
		}
    }

	return 0;
}
