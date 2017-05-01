// key.cpp nd_coin
/* keys are linked lists of prime numbers that allow us to store numbers much larger than the constraints of 
normal single longs or integers to exponentially increase security (from key.h)
implementation of the key class member functions: these will allow our key to "act" like a standard number
for our overall purposes in encryption */

#include "key.h"
#include <cstddef>
#include <iostream>

using namespace std;

node::node(const int &digit) :data(digit), next(nullptr)
{}

key::key(): head(nullptr) {size = 0;}

key::~key()
{
	if (head != nullptr)
	{
		node *curNode = head;

		while (curNode != nullptr)
		{
			node *nextNode = curNode->next;
			delete curNode;
			curNode = nextNode->next;
		}

	}
}

void key::pushfront(const int &newValue)
{
	node *newNode = new node(newValue);

	this->size += 1;
	newNode->next = head;
	head = newNode;
}

void key::print()
{
	node * p = head;
	while (p->next != nullptr)
		cout << p->data;
	cout << endl;
}

key key::operator=(const key &assign)
{
	node *traverseNode = assign.head;

	int data;
	while (traverseNode != nullptr)
	{
		data = traverseNode->data;
		this->pushfront(data);
		traverseNode = traverseNode->next;
	}

	this->size = assign.size;
	return *this;
}

key key::operator+(const key &other)
{
	key sum;
	int carry = 0;

	node *p1 = this->head;
	node *p2 = other.head;


	while (p1 != nullptr || p2 != nullptr)
	{
		if (p1 == nullptr || p2 == nullptr)
			break;
		else if ((p1->data + p2->data + carry) < 10)
		{
			sum.pushfront(p1->data + p2->data + carry);
			carry = 0;													// reset carry value
		}
		else
		{
			sum.pushfront((p1->data + p2->data) % 10);
			carry = 1;													// increase carry value because of a carry						
		}
			// iterate to next node
			p1 = p1->next;
			p2 = p2->next;
	}

	// If statements and while loops that account for different sized lists
	if (p1 == nullptr)
	{
		while (p2 != nullptr)
		{
			sum.pushfront(p2->data + carry);
			carry = 0;
			p2 = p2->next;
		}
	}
	else if (p2 == nullptr)
	{
		while (p1 != nullptr)
		{
			sum.pushfront(p1->data + carry);
			carry = 0;
			p1 = p1->next;
		}
	}

	*this = sum;											// this step avoids a seg fault when trying to return the key made in the scope of the function
	return *this;
}

key key::operator*(const key &other)
{
	key product, needToAdd;

	int carry = 0, holder = 0;								// carry used in addition, holder used for holding 0 place and so on when increasing multipliers
	int total, digit;										// used in adding digits to the needToAdd list/key

	node *p1 = this->head;
	node *p2 = other.head;

	while (p1 != nullptr)									// primary computation while loop
	{
		// make sure the placeholder for multiplication is correct
		for (int i = 0; i < holder; i++)
		{
			needToAdd.pushfront(0);
		}
		carry = 0;											// initialize carry to 0 for new product in secondary computation

		while (p2 != nullptr)								// secondary computation while loop
		{
			total = p2->data * p1->data;
			digit = (total % 10);
			needToAdd.pushfront(digit + carry);
			carry = (total - digit)/10;
		}
		holder += 1;										// update carry so that for loop above adds appropriate amount of placeholder 0s

		product = product + needToAdd;
	}

	*this = product;										// this step avoids a seg fault when trying to return the key made in the scope of the function
	return *this;
}

key key::operator-(const key &other)
{
	key difference;

	int carry = 0;

	node * p1 = this->head;
	node * p2 = other.head;

	if 

	while (p1->next != nullptr)
	{
		p1 = p1->next;										// iterate through to tail of the list to start from highest digit
	}
	while (p2->next != nullptr)
	{
		p2 = p2->next;
	}



	*this = difference;
	return *this;
}

int main()
{
	key key1, key2;

	int x = 1; int y = 2; //int z = 3;

	for (int i = 0; i < 2; i++)
	{
		key1.pushfront(x);
		key2.pushfront(y);
	}

	key1.print();
	//key2.print();
	/*
	key key3 = key1 + key2;
	key3.print();

	cout << "check3" << endl;
	key key4 = key1 * key2;
	key4.print();
	*/

	cout << "complete" << endl;
}



