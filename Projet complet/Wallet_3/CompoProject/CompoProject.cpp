// CompoProject.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Wallet.h"

using namespace std;


int main(int argc, char *argv[]){
	Wallet w("C:/Users/Jeremy/Desktop/test.json");
	std::cout << "argc" << argc << endl;
	std::cout << "LANCEMENT DU PROGRAMME WALLET" << std::endl;
	if (argc == 2 && 0 == strcmp(argv[1], "resume")) {
		w.solde();
	}
	else if (argc==4 && 0==strcmp(argv[1],"transaction")) {


		//transforme les arguments pour appeller la fonction transaction

		string clePrivee(argv[2]);
		string clePublique (argv[3]);
		int montant = atoi(argv[4]);
		cout << "cle prive : " << clePrivee;
		//w.depenser(clePublique,clePrivee, montant);
	}
	
	return 0;
}