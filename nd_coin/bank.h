#ifndef BANK_H
#define BANK_H

#include <map>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

struct account{
	//int balance;
	vector<int> serial_numbers;
};

class bank{
	public:
		bank();
		void giveCoin(string,int);
		bool takeCoin(string,int);
		void setBalance(string,int);
		int getBalance(string);
		int takeCoin(string);
	private:	
		map<string,account> accounts;
		vector<int> coins;
};

#endif
