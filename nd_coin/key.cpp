// key.cpp nd_coin
/* keys are linked lists of prime numbers that allow us to store numbers much larger than the constraints of 
normal single longs or integers to exponentially increase security (from key.h)
implementation of the key class member functions: these will allow our key to "act" like a standard number
for our overall purposes in encryption */

#include "key.h"
#include <cstddef>
#include <stdlib>
#include <iostream>

using namespace std;

node::node(const int &digit) :data(digit), next(nullptr)
{}

key::key(): head(nullptr) {}

key::~key()
{
	if (head != nullptr)
	{
		node *curNode = head;

		while (curNode != nullptr)
		{
			node *nextNode = curNode->next;
			delete curNode;
			curNode = nextNode;
		}

	}
}

void key::pushfront(const int &newValue)
{
	node *newNode = new node(newValue);

	newNode->next = head;
	head = newNode;
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

	return *this;
}

key key::operator+(const key &other)
{
	key sum;
	int carry = 0;

	node *p1 = this->head;
	node *p2 = this->head;


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
	*this = sum;
	return *this;
}

key key::operator*(const key &other)
{
	key product;

	int
}


int main()
{

}
