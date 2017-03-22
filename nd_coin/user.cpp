#include "user.h"
#include <fstream>

user::user(){
	ifstream ifs;
	string data;
	int phi_n,n,exponent,prime1,prime2;
	vector<int> phi_n_factors;
	vector<int> exponent_factors;
	vector<int>::iterator phi_it;
	vector<int>::iterator exp_it;


	//primes.txt contains all primes length 6
	ifs.open("./primes/primes.txt");
	ofstream temp("./primes/temp.txt");	
	// get 2 prime numbers length 6
	getline(ifs,data);
	prime1 = atoi(data.c_str());
	getline(ifs,data);
	prime2 = atoi(data.c_str());
	//Update the text file to no longer include used primes
	while (!ifs.eof())
	{
		getline(ifs,data);
		temp << data << endl;
	}
	ifs.clear();			// clear the eof and fail bits
	ifs.close();
	temp.close();
	// delete old file, rename temp file to primes.txt to "update" primes.txt
	remove("./primes/primes.txt");
	rename("./primes/temp.txt", "./primes/primes.txt");
	
	n = prime1 * prime2;
	phi_n = (prime1-1) * (prime2-1);

	//find all factors of phi_n
	for(int i=2;i<phi_n;i++){
		if(phi_n%i==0){
			phi_n_factors.push_back(i);
		}
	}

//cout<<(1 + (2 * rand ()) %10)<<endl;
	
	//exponent is a small random odd number not a sharing factor with phi n
	//ensure exponent odd and doesnt share factor
	bool check = false;
	while(check==false){
		//rand odd digit 1-10
		exponent = 1 + (2 * rand ()) %10;
		//ensure positve
		if(exponent<0)
			exponent*=-1;

		check = true;

		//get all factors of exponent
		for(int i=2;i<exponent;i++){
			if(exponent%i==0){
				exponent_factors.push_back(i);
			}
		}

		phi_it = phi_n_factors.begin();
		exp_it = exponent_factors.begin();

		int count =0;

		//loop through all factors 
		while(phi_it!=phi_n_factors.end()){
			while(exp_it!=exponent_factors.end()){
				//if they share a factor break
				if(*exp_it == *phi_it){
					check = false;
					//break;
				}
				count++;
				exp_it++;
			}
			phi_it++;
		}
	}

	cout<<exponent<<endl<<phi_n<<endl;
}

int user::getPubKey(){
	return publicKey;
}