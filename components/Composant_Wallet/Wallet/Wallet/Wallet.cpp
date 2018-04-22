
#include "stdafx.h"
#include <vector>
#include <iostream>
#include "Bloc.h"
#include "Wallet.h"
#include <time.h>
#include <string>

using namespace std;

class Verificateur {

};

class Hasheur {

};

class Signature {

};

class Mineur
{
};


class FileInterface {
private:
	std::string fichier;
	void write(std::string str);

	json toJson(TXI txi);
	json toJson(UTXO utxo);
	json toJson(TX tx);
	json toJson(TXM txm);
	json toJson(Bloc bloc);
	json toJson(std::vector<Bloc> blocs);

	std::vector<Bloc> parse(std::string blocsJson);
	Bloc parseBloc(json j);
	TXM parseTxm(json j);
	TX parseTx(json j);
	UTXO parseUtxo(json j);
	TXI parseTxi(json j);

public:
	// Constructeur : initialisation la classe et lit le fichier json passé en paramètre
	FileInterface(std::string fichier);

	// Insert le bloc à la suite du fichier - génére une exception si le hash du bloc précédent n'est pas bon
	void insert(Bloc bloc);

	// Recherche un bloc en fonction de son hash - génére une exception si aucun bloc n'est trouvé
	Bloc findByHash(std::string hash);

	// Recherche un bloc en fonction de sa position dans la chaine - génére une exception si aucun bloc n'est trouvé
	Bloc findByIndex(int index);

	// Parcourt tous les blocs afin de savoir s'ils sont conformes - génére une exception si un n'est pas conforme
	void verification();

	// Retourne tous les blocs constituant la blockchain sous forme de vecteur
	std::vector<Bloc> readAll();

	// Retourne un bloc sous forme de chaine de caractère. Si hash == true, renvoie la chaine de caracère avec le hash et le nonce, sinon sans le hash et le nonce
	 std::string toString (bool hash);

};

class Wallet {
public:
	Verificateur verificateur;
	string filePath;
	Mineur mineur;
	FileInterface BCFile;
	vector<Bloc> blocs;
	string publicKey;
	string privateKey;
	Signature signature;
	Hasheur hasheur;
	Wallet() {
		BCFile = FileInterface("");
		blocs = BCFile.readAll();
		publicKey = "";
		privateKey = "";
	}
	Wallet(string pathFile,string ppublicKey,string pprivateKey){
		BCFile = FileInterface(pathFile);
		blocs = BCFile.readAll();
		publicKey = ppublicKey;
		privateKey = pprivateKey;
		verificateur = Verificateur(BCFile);
	}	
	int solde() {
		
		int solde = 0;

		for (int i = 0; i < blocs.size;i++) {
			Bloc b = blocs[i];
			UTXO utxo0 = b.tx1.utxo[0];
			UTXO utxo1 = b.tx1.utxo[1];
			//on m a envoye de l argent
			if (utxo0.dest.compare(publicKey)==0) {
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
			if (utxo1.dest.compare(publicKey)==0) {
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
	void depenser(int destinatire, int montant) { 
		if (this->solde() < montant) {
			cout << "Solde insuffisant ! " << endl;
		}
		else {
			int solde = 0;
			vector<TXI> TXIs;

			for (int i = 0; i < blocs.size; i++) {
				Bloc b = blocs[i];
				UTXO utxo0 = b.tx1.utxo[0];
				UTXO utxo1 = b.tx1.utxo[1];
				//on m a envoye de l argent
				if (utxo0.dest == publicKey) {
					solde += utxo0.montant;
					TXI txi;
					txi.nBloc = i;
					txi.nUtxo = 0;
					//signature à faire
					//	string sign(string data, string privKey);
					string stringdata = to_string(montant) + publicKey + to_string(i) + to_string(0);
					string hash = hasheur.hash(stringdata);
					txi.signature= signature.sign(hash,privateKey);
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
						newUtxo0.dest = destinatire;

						string stringdata = to_string(montant) + publicKey + to_string(i) + to_string(0);
						string hash = hasheur.hash(stringdata);

						newUtxo0.hash = hash;
						tx.utxo[0] = newUtxo0;
						
						// me rendre la monnaie
						UTXO newUtxo1;
						newUtxo1.montant = solde-montant;
						newUtxo1.dest = publicKey;

						string stringdatam = to_string(montant) + publicKey + to_string(i) + to_string(1);
						string hashm = hasheur.hash(stringdatam);
						newUtxo1.hash = hashm;
						tx.utxo[1] = newUtxo1;						
						Bloc newBloc = mineur.creerBloc(tx, blocs[blocs.size].hash);
						blocs.push_back(newBloc);
						bool ok = verificateur.checkBlockchain(blocs);
						if (ok) {
							BCFile.insert(newBloc);
						}
					}
				}	
				//
				if (utxo1.dest == publicKey) {
					solde += utxo1.montant;
					TXI txi;
					txi.nBloc = i;
					txi.nUtxo = 0;
					//signature à faire
					//	string sign(string data, string privKey);
					string stringdata = to_string(montant) + publicKey + to_string(i) + to_string(0);
					string hash = hasheur.hash(stringdata);
					txi.signature = signature.sign(hash, privateKey);
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
						newUtxo0.dest = destinatire;

						string stringdata = to_string(montant) + publicKey + to_string(i) + to_string(0);
						string hash = hasheur.hash(stringdata);

						newUtxo0.hash = hash;
						tx.utxo[0] = newUtxo0;

						// me rendre la monnaie
						UTXO newUtxo1;
						newUtxo1.montant = solde - montant;
						newUtxo1.dest = publicKey;

						string stringdatam = to_string(montant) + publicKey + to_string(i) + to_string(1);
						string hashm = hasheur.hash(stringdatam);
						newUtxo1.hash = hashm;
						tx.utxo[1] = newUtxo1;
						Bloc newBloc = mineur.creerBloc(tx, blocs[blocs.size].hash);
						blocs.push_back(newBloc);
						bool ok = verificateur.checkBlockchain(blocs);
						if (ok) {
							BCFile.insert(newBloc);
						}												
						break;
					}
				}
			}
		}		
	}	
};
int main()
{
	Wallet  w;
	Bloc b1;
	w.blocs.push_back(b1);
	TXM txm;
	UTXO utxo0;
	utxo0.dest = 1;
	utxo0.montant = 200;
	utxo0.hash = 201;// hash(nBloc,nTx,nUTXO,montant,destinataire) pour securisation de l'UTXO
	txm.utxo[0] = utxo0;
	b1.tx0 = txm;

	UTXO utxo1;
	utxo1.dest = 2;
	utxo1.montant = 35;
	utxo1.hash = 36;

	UTXO utxo2;
	utxo2.dest = 1;
	utxo2.montant = 88;
	utxo2.hash = 89;

	TXI txi;
	txi.nBloc = 0;
	txi.nTx = 0;
	txi.nUtxo = 0;

	TX tx;
	tx.TXIs.push_back(txi);
	tx.utxo[0] = utxo1;
	tx.utxo[1] = utxo2;



	////////////
	int c;
	cin >> c;
    return 0;

}