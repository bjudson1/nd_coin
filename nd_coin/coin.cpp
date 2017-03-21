
void signCoin(string pub_key){
	sethash(pub_key);
}

string getHash(){
	return hash;
}

void sethash(string input){
	int index=0;
	string new_hash = "";

	while(index<input.length()){
		new_hash += int(input[index]) + 5;
		i++;
	}

	hash = new_hash;
}


