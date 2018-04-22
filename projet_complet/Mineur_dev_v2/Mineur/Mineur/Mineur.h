#pragma once
//
//  Mineur.h
//  Mineur
//
//  Created by florian bayeux on 26/03/2018.
//  Copyright © 2018 cours. All rights reserved.
//
#ifndef Mineur_h
#define Mineur_h
#ifdef MINEUR_EXPORTS
#define MINEUR_EXPORTS_INTERFACE __declspec(dllexport)
#else
#define MINEUR_EXPORTS_INTERFACE __declspec(dllimport)
#endif


#include <string>
#include "Bloc.h"
#include "Hacheur.h"

#define NONCE_MAX 1000


class MINEUR_EXPORTS_INTERFACE Mineur 
{

protected:
	// Le nombre de 0 dont doit commencer la châine hachée
	int m_nombreZeros;

	//Le bloc incomplet reçu par le module Wallet
	Bloc m_bloc;

public:
	Mineur(Bloc bloc, int nombreZeros);
	/*
	 *  utilise le bloc et le nombre de 0 pour effectuer le hash
	 *  et le mettre dans l’attribut hash
	 *  @Return: boolean(true en cas de succès et false sinon)
	 */
	bool transform();
	/*
	 *  retourne le bloc complet
	 *  @Return: Bloc
	 */
	Bloc getM_bloc();


};
#endif /* Mineur_h */
