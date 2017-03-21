#ifndef USER_H
#define USER_H

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

class user{
	public:
		user();
		int getPubKey();
		int publicKey;
	private:	
		int privateKey;
};

#endif