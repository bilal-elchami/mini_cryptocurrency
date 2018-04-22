#include "Bloc.h"
#include "stdafx.h"
#include "Wallet.h"


using namespace std;


void stringToUnsignedChar64(string s, unsigned char*& uc) {
	for (int i = 0; i < s.length(); i++) {
		uc[i] = s[i];
	}
	return;
}


void Wallet::depenser(string publicDestinataire, string publicEmetteur, int montant) {
	if (solde(publicEmetteur) < montant) {
		cout << "Solde insuffisant ! " << endl;
	}
	else {
		int solde = 0;
		vector<TXI> TXIs;
		for (int i = 0; i < blocs.size(); i++) {
			Bloc b = blocs[i];
			UTXO utxo0 = b.tx1.UTXOs[0];
			UTXO utxo1 = b.tx1.UTXOs[1];
			//on m a envoye de l argent
			if(strcmp(publicDestinataire.c_str(), (const char *)utxo0.dest)==0){
				solde += utxo0.montant;
				TXI txi;
				txi.nBloc = i;
				txi.nUtxo = 0;
				string stringdata = to_string(montant) + publicDestinataire + to_string(i) + to_string(0);
				string hash = hasheur.hacher(stringdata);//need other composant
				string s = signature.signMessage(hash, privateKeys[i]);
				for (int k = 0; k < s.length(); k++) txi.signature[k] = s[k];
				TXIs.push_back(txi);
				for (Bloc b0 : blocs) {
					for (TXI txi : b0.tx1.TXIs) {
						//si utxo0 a ete depense
						if (txi.nBloc == i &&  txi.nUtxo == 0) {
							solde -= utxo0.montant;
							TXIs.pop_back();
						}
					}
				}
				if (solde > montant) {
					TX tx;
					tx.TXIs = TXIs;
					// payer le destinataire
					UTXO newUtxo0;
					newUtxo0.montant = montant;
					for (int k = 0; k < publicDestinataire.size(); k++) newUtxo0.dest[k] = publicDestinataire[k];
					//newUtxo0.dest = uu;

					string stringdata = to_string(montant) + publicDestinataire + to_string(i) + to_string(0);
					string hashTmp = hasheur.hacher(stringdata);
					for (int k = 0; k < hashTmp.length(); k++) newUtxo0.hash[k] = hashTmp[k];
					tx.UTXOs[0] = newUtxo0;

					// me rendre la monnaie
					UTXO newUtxo1;
					newUtxo1.montant = solde - montant;
					for (int k = 0; k < publicDestinataire.size(); k++) newUtxo1.dest[k] = publicDestinataire[k];
					string stringdatam = to_string(montant) + publicDestinataire + to_string(i) + to_string(1);
					string hashm = hasheur.hacher(stringdatam);
					for (int k = 0; k < hashm.length(); k++) newUtxo0.dest[k] = hashm[k];
					tx.UTXOs[1] = newUtxo1;
					//Bloc newBloc = mineur.CreerBloc(tx, (unsigned char *)blocs[blocs.size()].hash);//need to be implemented
					Bloc newBloc;
					blocs.push_back(newBloc);

					Wallet::BCFile.verification();
					BCFile.insert(newBloc);
				}
			}
			//
			if(strcmp(publicDestinataire.c_str(), (const char *)utxo1.dest)==0) {
				solde += utxo1.montant;
				TXI txi;
				txi.nBloc = i;
				txi.nUtxo = 0;
				//signature à faire
				//	string sign(string data, string privKey);
				hasheur = Hacheur();
				string stringdata = to_string(montant) + publicDestinataire + to_string(i) + to_string(0);
				string hash = hasheur.hacher(stringdata);//need other composant
				string s = signature.signMessage(hash, privateKeys[i]);
				for(int k = 0; k < s.length(); k++) txi.signature[k] = s[k];
				TXIs.push_back(txi);
				for (Bloc b0 : blocs) {
					for (TXI txi : b0.tx1.TXIs) {
						//si utxo0 a ete depense
						if (txi.nBloc == i &&  txi.nUtxo == 0) {
							solde -= utxo1.montant;
							TXIs.pop_back();
						}
					}
				}
				if (solde > montant) {
					TX tx;
					tx.TXIs = TXIs;
					// payer le destinataire
					UTXO newUtxo0;
					newUtxo0.montant = montant;
					for (int k = 0; k < publicDestinataire.size(); k++) newUtxo0.dest[k] = publicDestinataire[k];
					string stringdata = to_string(montant) + publicDestinataire + to_string(i) + to_string(0);
					string hash = hasheur.hacher(stringdata);
					for (int k = 0; k < hash.length(); k++) newUtxo0.hash[k] = hash[k];
					tx.UTXOs[0] = newUtxo0;

					// me rendre la monnaie
					UTXO newUtxo1;
					newUtxo1.montant = solde - montant;
					for (int k = 0; k < publicDestinataire.size(); k++) newUtxo1.hash[k] = publicDestinataire[k];
					string stringdatam = to_string(montant) + publicDestinataire + to_string(i) + to_string(1);
					string hashm = hasheur.hacher(stringdatam);
					for (int k = 0; k < hashm.length(); k++) newUtxo1.hash[k] = hashm[k];
					tx.UTXOs[1] = newUtxo1;
					//Bloc newBloc = mineur.CreerBloc(tx, (unsigned char *)blocs[blocs.size()-1].hash);//need other composant
					Bloc newBloc;
					blocs.push_back(newBloc);
					BCFile.verification();
					BCFile.insert(newBloc);
					break;
				}
			}
		}
	}
}


string  Wallet::solde()
{
	stringstream ss;
	for (int i = 0; i < (int)(blocs.size()); i++)
	{
		for (int j = 0; j < blocs[i].tx1.UTXOs.size(); j++)
		{
			ss << blocs[i].tx1.UTXOs[j].dest << " : " << blocs[i].tx1.UTXOs[j].montant << endl;
		}
	}
	return ss.str();
}

int MyCompare(string a, unsigned char* b)
{
	if (a.length() != strlen((char*)b)) return 0;
	for (int i = 0; i < a.length(); i++)
	{
		if (a[i] != b[i]) return 0;
	}
	return 1;
}

int Wallet::solde(string publicKey)
{

	int solde = 0;

	for (int i = 0; i < blocs.size(); i++)
	{
		Bloc b = blocs[i];
		UTXO utxo0 = b.tx1.UTXOs[0];
		UTXO utxo1 = b.tx1.UTXOs[1];
		//on m a envoye de l argent
		if (MyCompare(publicKey, utxo0.dest) == 0) {
			solde += utxo0.montant;
			for (Bloc b0 : blocs) {
				for (TXI txi : b0.tx1.TXIs) {
					//si utxo0 a ete depense
					if (txi.nBloc == i &&  txi.nUtxo == 0) {
						solde -= utxo0.montant;
					}
				}
			}

		}
		//je me rends la monnaie
		if(MyCompare(publicKey,utxo1.dest)) {
			solde += utxo0.montant;
			for (Bloc b0 : blocs) {
				for (TXI txi : b0.tx1.TXIs) {
					//si utxo0 a ete depense
					if (txi.nBloc == i && txi.nUtxo == 1) {
						solde -= utxo1.montant;
					}
				}
			}
		}
	}
	return solde;
}





Wallet::Wallet(string pathFile)
{
	//BCFile = FileInterface("");
	//blocs = BCFile.readAll(); 
	//publicKeys = "";
	//privateKeys = "";
}
