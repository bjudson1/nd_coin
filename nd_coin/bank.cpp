#include "bank.h"

bank::bank(){

}

void bank::setBalance(string account_name,int amount){
	map<string,int>::iterator it = accounts.begin();

	accounts.insert(it,pair<string,int>(account_name,amount));
}

int bank::getBalance(string name){
	return accounts.find(name)->second;
}