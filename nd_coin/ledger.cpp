#include "ledger.h"

ledger::ledger(){
	count = 0;
}

void ledger::putMessage(string incoming_message){
	messages.push(incoming_message);
	count++;
}

void ledger::makeTransfer(int serial_num){
	
}

stack<string> ledger::getMessages(){
	return messages;
}


int ledger::getSize(){
	return count;
}

void ledger::print(){
	cout<<messages.top();
}