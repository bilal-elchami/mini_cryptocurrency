#pragma once
#include <vector>
#include <iostream>
#include <time.h>
#include <string>
#include <sstream>
#include "Bloc.h"
#include "FileInterfaceAPI.h"

using namespace std;

class Verificateur
{
public:
	Verificateur();
	Verificateur(FileInterface fifi);
};
class Mineur
{
public :
	Mineur();
	Bloc CreerBloc(TX tx, unsigned char hash[64]);
	
};
class KeyChain {
public:
	KeyChain(string private_key, string public_key);
	void save();
	string private_key;
	string public_key;
};
class Signature
{
public:
	Signature();
	KeyChain generateKeys();
	string signMessage(string data, string private_key) {
		return "falseKey";
	}
	bool validateSignature(string data, string public_key, string signature);
};
class Hasheur { 
public :
	Hasheur();
	string hash(string stringData) {
		return "FalseHash";
	}
};

class Wallet {

public:
	FileInterface BCFile;
	Verificateur verificateur;
	std::string filePath;
	Mineur mineur;
	vector <std::string> publicKeys;
	vector <std::string> privateKeys;
	Signature signature;
	Hasheur hasheur;
	
	vector<Bloc> blocs;

	int solde(std::string publicKey);
	std::string  solde();

	void depenser(std::string publicDestinatire, std::string publicEmetteur, int montant);

	Wallet(std::string pathFile);

};