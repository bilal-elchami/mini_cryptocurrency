#pragma once
#ifndef _BLOC_H

#include <string>
#include <vector>

//
// bloc.h  version 1.1
//
// projet blockchain M2IF 2018
//

#define KEY_SIZE 64 // taille en byte des cle (publique, privee) a valider avec les groupes specifiant et developpant la signature
#define HASH_SIZE 64  // taille du hash, 256 bits chaine hexadecimale
#define SIGNATURE_SIZE 64
using namespace std;
class TXI // input d'une transaction
{
 public:
	unsigned int nBloc; // block number of the source Utxo
	unsigned int nTx;   // hash of the previous transaction
	unsigned int nUtxo; // index of output ( la référence d'une transaction précédente)
	unsigned char signature[64]; // signature de la transaction avec la clé privée
};

class UTXO // une sortie non dépensée
{
 public:
  int nBloc;
  int nTx;
  int nUTX0;         // ces 3 champs nBloc,nTx,nUTX0) forment l'identifiant unique de l'UTXO
  float montant;
  unsigned char dest[KEY_SIZE];  //	compte destinataire (clé publique)
  unsigned char hash[HASH_SIZE];    // hash(nBloc,nTx,nUTXO,montant,destinataire) pour securisation de l'UTXO

  std::string toString();
};

class TX { // transaction standard (many inputs, many outputs)
 public:
  std::vector<TXI>	TXIs;
  std::vector<UTXO>	UTXOs;
};

class TXM { // transaction du mineur
 public:
	UTXO utxo[1];
};

class Bloc
{
 public:
	char hash[HASH_SIZE]; // hash des autres champs, hash of the entire bloc
	unsigned int nonce;

	char previous_hash[HASH_SIZE]; // hash of the previous bloc
	int num; // numero du bloc, commence a zero
	TX tx1; //  transaction du bloc
	TXM tx0; // transaction du mineur (coinbase)
	string ToString() {
		string str = "{";
		str += "\n\t\"hash\": \"" + string(hash);
		str += "\",\n\t\"nonce\": \"" + to_string(nonce) + "\",";
		str += "\n\t\"previous_hash\": \"" + string(previous_hash);
		str += "\",\n\t\"num\": \"" + to_string(num);
		str += "\",\n\t\"TX\": { \n\t\t";
		str += "\"TXI\": [{";

		for (unsigned int i = 0; i < tx1.TXIs.size(); i++) {
			str += "\"nBloc\": \"" + to_string(tx1.TXIs[i].nBloc);
			str += "\",\"nUTXO\": \"" + to_string(tx1.TXIs[i].nUtxo);
			str += "\",\"nTx\": \"" + to_string(tx1.TXIs[i].nTx);
			str += "\",\"signature\": \"" + string(tx1.TXIs[i].signature, tx1.TXIs[i].signature + sizeof tx1.TXIs[i].signature / sizeof tx1.TXIs[i].signature[0]);
			str += "\"}";
			if (i != (tx1.TXIs.size() - 1)) {
				str += ",";
			}
		}
		str += "],\n";
		str += "\"UTXO\": [{";
		for (unsigned int i = 0; i < tx1.UTXOs.size(); i++) {
			str += "\"nBloc\": \"" + to_string(tx1.UTXOs[i].nBloc);
			str += "\",\"nUTXO\": \"" + to_string(tx1.UTXOs[i].nUTX0);
			str += "\",\"nTx\": \"" + to_string(tx1.UTXOs[i].nTx);
			str += "\",\"montant\": \"" + to_string(tx1.UTXOs[i].montant);
			str += "\",\"cle_publique\": \"" + string(tx1.UTXOs[i].dest, tx1.UTXOs[i].dest + sizeof tx1.UTXOs[i].dest / sizeof tx1.UTXOs[i].dest[0]);
			str += "\",\"hash\": \"" + string(tx1.UTXOs[i].hash, tx1.UTXOs[i].hash + sizeof tx1.UTXOs[i].hash / sizeof tx1.UTXOs[i].hash[0]);
			str += "\"}";
			if (i != (tx1.UTXOs.size() - 1)) {
				str += ",";
			}
		}

		str += "]\n},";
		str += "\",\n\t\"TXM\": {";
		str += "\n\t\t\"nBloc\": \"" + to_string(tx0.utxo[0].nBloc);
		str += "\",\n\t\"nUTXO\": \"" + to_string(tx0.utxo[0].nUTX0) + "\",";
		str += "\",\n\t\"montant\": \"" + to_string(tx0.utxo[0].montant) + "\",";
		str += "\",\n\t\"cle_publique\": \"" + string(tx0.utxo[0].dest, tx0.utxo[0].dest + sizeof tx0.utxo[0].dest / sizeof tx0.utxo[0].dest[0]) + "\",";
		str += "\",\n\t\"hash\": \"" + string(tx0.utxo[0].hash, tx0.utxo[0].hash + sizeof tx0.utxo[0].hash / sizeof tx0.utxo[0].hash[0]) + "\"\n\t}\n}";
		return str;
	}
};

#endif
