#pragma once
#include <vector>
#include <iostream>
#include <time.h>
#include <string>
#include <sstream>
#include "Bloc.h"
#include "Json.h"
#include "Signature.h"
#include "Hacheur.h"


using json = nlohmann::json;

using namespace std;


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
	FileInterface() 
	{};


	// Constructeur : initialisation la classe et lit le fichier json passé en paramètre
	FileInterface(std::string fichier);
	// Insert le bloc à la suite du fichier - génére une exception si le hash du bloc précédent n'est pas bon
	void insert(Bloc bloc) {}

	// Recherche un bloc en fonction de son hash - génére une exception si aucun bloc n'est trouvé
	Bloc findByHash(std::string hash);

	// Recherche un bloc en fonction de sa position dans la chaine - génére une exception si aucun bloc n'est trouvé
	Bloc findByIndex(std::string hash);

	// Parcourt tous les blocs afin de savoir s'ils sont conformes - génére une exception si un n'est pas conforme
	void verification() {}

	// Retourne tous les blocs constituant la blockchain sous forme de vecteur
	std::vector<Bloc> readAll();

	// Retourne un bloc sous forme de chaine de caractère. Si hash == true, renvoie la chaine de caracère avec le hash et le nonce, sinon sans le hash et le nonce
	std::string toString(bool hash);

};

class Verificateur
{
public:
	Verificateur() {};
	Verificateur(FileInterface fifi);
};
class Mineur
{
public :
	Mineur() {};
	Bloc CreerBloc(TX tx, unsigned char hash[64]);
	
};
/*class KeyChain {
public:
	KeyChain(string private_key, string public_key);
	void save();
	string private_key;
	string public_key;
};
class Signature
{
public:
	Signature() {};
	KeyChain generateKeys();
	string signMessage(string data, string private_key) {
		return "falseKey";
	}
	bool validateSignature(string data, string public_key, string signature);
};
class Hasheur { 
public :
	Hasheur() {};
	string hash(string stringData) {
		return "FalseHash";
	}
};*/

class Wallet {

public:
	FileInterface BCFile;
	Verificateur verificateur;
	std::string filePath;
	Mineur mineur;
	vector <std::string> publicKeys;
	vector <std::string> privateKeys;
	Signature signature;
	Hacheur hasheur;
	
	vector<Bloc> blocs;

	int solde(std::string publicKey);
	std::string  solde();

	void depenser(std::string publicDestinatire, std::string publicEmetteur, int montant);

	Wallet(std::string pathFile);

};