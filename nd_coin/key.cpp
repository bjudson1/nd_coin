// key.cpp nd_coin
/* keys are linked lists of prime numbers that allow us to store numbers much larger than the constraints of 
normal single longs or integers to exponentially increase security (from key.h)
implementation of the key class member functions: these will allow our key to "act" like a standard number
for our overall purposes in encryption */

#include "key.h"

using namespace std;

node::node(const int &digit) :data(digit), next(NULL)
{}

