#ifndef BANK_H
#define BANK_H

#include <map>
#include <string>

using namespace std;

class bank{
	public:
		bank();
		void setBalance(string,int);
		int getBalance(string);
	private:	
		map<string,int> accounts;
};

#endif
