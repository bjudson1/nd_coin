// key.h nd_coin
// keys are linked lists of prime numbers that allow us to store numbers much larger than the constraints of 
// normal single longs or integers to exponentially increase security 
#ifndef _KEY_H_
#define _KEY_H_

using namespace std;

// Node description: used as single digit holders in the linked list implementation of key
struct node {
	node (const int &);						// constructor
	int data;
	node *next;
};

// key description: implemented as a linked list where head points to "first digit" of the overall number
class key {
	public:
		// methods 
		key();								// constructor				
		~key();								// deconstructor
		void pushfront(const int &);		// adds a new digit to the key

		// overloaded operators
		key operator=(const key &);
		key operator+(const key &);
		key operator*(const key &);
		// key operator-(const key &); ?
		// key operator/(const key &); ?

		// members
		node *head;
		node *newNode(const int &);			// helper function, creates a new node
};

#endif