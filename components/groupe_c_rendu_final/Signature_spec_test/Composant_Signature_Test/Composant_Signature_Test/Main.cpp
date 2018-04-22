#include <iostream>
using namespace std;

#include "KeyChain.h"
#include "Signature.h"

bool testValidateSignature()
{
	KeyChain key = Signature::generateKeys();
	string data = "projet composant";
	key.save();

	string signature_result = Signature::signMessage(data, key.private_key);
	string signature_result_false = Signature::signMessage("BlaBla", key.private_key);

	cout << "Signature 1 : " << signature_result << endl;
	cout << "Signature 2 : " << signature_result_false << endl << endl;

	bool isValidated = Signature::validateSignature(data, key.public_key, signature_result);
	bool isValidated_false = Signature::validateSignature(data, key.public_key, signature_result_false);

	cout << isValidated << endl;
	cout << isValidated_false << endl;


	if (isValidated == true && isValidated_false == false)
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool testSignMessage()
{
	KeyChain key = Signature::generateKeys();
	key.save();

	KeyChain key2 = Signature::generateKeys();
	key2.save();

	string data = "projet composant";

	string signature_result = Signature::signMessage(data, key.private_key);
	string signature_result1 = Signature::signMessage(data, key2.private_key);

	if (signature_result != signature_result1)
	{
		return true;
	}
	else
	{
		return false;
	}

}

void main()
{

	cout << "Programme de test du composant signature" << endl;
	if (testValidateSignature() == true)
	{
		cout << "TestValidateSignature OK" << endl;
	}
	else
	{
		cout << "TestValidateSignature FAILED" << endl;
	}
	if (testSignMessage() == true)
	{
		cout << "TestSignMessage OK" << endl;
	}
	else
	{
		cout << "TestSignMessage FAILED" << endl;
	}

	system("PAUSE");

}
