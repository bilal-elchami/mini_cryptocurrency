#include <assert.h> 

#include "stdafx.h"
#include "Wallet.h"






void test_depenser() {
	unsigned char *user1 = new unsigned char[256]();
	unsigned char *user2 = new unsigned char[256]();
	unsigned char *user3 = new unsigned char[256]();

	user1[0] = 'd';
	user1[1] = 'e';
	user1[2] = 's';
	user1[3] = 't';

	user2[0] = 'e';
	user2[1] = 'm';
	user2[2] = 'e';
	user2[3] = 't';

	assert(solde(user1) == 0);

	Wallet  w;

	Bloc b1;
	w.blocs.push_back(b1);
	TXM txm;

	UTXO utxo0;
	utxo0.dest = user1;
	utxo0.montant = 1000;
	utxo0.hash = "";

	txm.utxo[0] = utxo0;
	b1.tx0 = txm;

	depenser(user1, user2, 100);
	depenser(user2, user1, 100);
	assert(solde(user1) == 1000); // verifie que le destinataire n'a pas recu d'argent (parce que l'emetteur n'en a pas)

	assert(solde(user2) == 100); // verifie que le destinataire a recu l'argent
	assert(solde(user1) == 900); // verifie que le montant a ete retire du compte de l'emetteur
	delete[] user1;
	delete[] user2;
	delete[] user3;


}


void test_solde_par_user() {
	unsigned char *user1 = new unsigned char[256]();

	user1[0] = 'd';
	user1[1] = 'e';
	user1[2] = 's';
	user1[3] = 't';

	assert(solde(user1) == 0); // verifie qu'un user a un solde nul au debut

	Wallet  w;

	Bloc b1;
	w.blocs.push_back(b1);
	TXM txm;

	UTXO utxo0;
	utxo0.dest = user1;
	utxo0.montant = 100;
	utxo0.hash = "";

	txm.utxo[0] = utxo0;
	b1.tx0 = txm;

	assert(solde(user1) == 100); // verifie que le montant a bien ete pris en comptes

	delete[] user1;


}
