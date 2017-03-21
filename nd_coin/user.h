#ifndef USER_H
#define USER_H

using namespace std;

class user{
	public:
		string pub_key;
	private:	
		string pri_key;
		string hashFunc(string,string);
};

#endif