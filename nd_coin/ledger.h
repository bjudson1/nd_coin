#ifndef LEDGER_H
#define LEDGER_H

#include <stack>
#include <string>
#include <iostream>

using namespace std;

class ledger{
	public:
		ledger();
		void putMessage(string);
		void makeTransfer(int);
		stack<string> getMessages();
		int getSize();
		void print();
	private:	
		int count;
		stack<string> messages;
};

#endif
