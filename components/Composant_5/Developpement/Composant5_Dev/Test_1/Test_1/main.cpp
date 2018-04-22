#include <iostream>

#include "FileInterface.h"
#include "Composant5.h"

using namespace std;

int main(int argc, char* argv[]) {
	cout << "TEST COMPOSANT 5" << endl;
	FileInterface f("blocInitial.json"); // attention au path du projet
	Bloc blocInitial = f.findByIndex(1);
	CComposant5 c;
	/*
		Test pour avoir le hash du bloc initial (recuperation via deboggage)
	*/
	if (c.verify_bloc(blocInitial, blocInitial.hash, blocInitial.nonce)) {
		cout << "hash bloc valide" << endl;
	}
	else {
		cout << "hash bloc invalide" << endl;
	}
	/*
		Test pour avoir le hash du TXM du bloc initial (recuperation via deboggage)
	*/
	if (c.verify_transaction(blocInitial)) {
		cout << "hash txm valide" << endl;
	}
	else {
		cout << "hash txm invalide" << endl;
	}
	return 0;
}