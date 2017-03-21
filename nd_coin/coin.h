#ifndef COIN_H
#define COIN_H

using namespace std;

class coin{
	public:
		coin();
		void signCoin();
		string getHash();
	private:	
		string hash;
		void setHash(string,string);
};

#endif