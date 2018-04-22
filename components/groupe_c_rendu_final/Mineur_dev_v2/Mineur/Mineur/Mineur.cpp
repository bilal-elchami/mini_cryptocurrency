//
//  Mineur.cpp
//  Mineur
//
//  Created by florian bayeux on 26/03/2018.
//  Copyright © 2018 cours. All rights reserved.
//

#include <stdio.h>
#include "Mineur.h"
#include <bitset>

using namespace std;
int retourNb0(string bloc_hashe){
    int nb = 0 ;
    for(int i = 0 ; i < HASH_SIZE;i++ ){
        if( bloc_hashe[i]=='0'){
            nb +=4;
        }else if(bloc_hashe[i]=='1') {
            return nb + 3;
        }else if (bloc_hashe[i]=='2' || bloc_hashe[i]=='3'){
            return nb + 2;
        }else if (bloc_hashe[i]=='4' || bloc_hashe[i]=='5' || bloc_hashe[i]=='6' ||bloc_hashe[i]=='7'){
            return nb + 1;
        }else{
            return nb;
        }
    }
    return nb;
}
Mineur::Mineur(Bloc bloc, int nombreZeros) {
    m_bloc = bloc;
    m_nombreZeros = nombreZeros;
    bloc.nonce= 0;
    for(int i = 0 ; i < HASH_SIZE;i++){
        bloc.hash[i]=0;
    }
}

bool Mineur::transform() {
    // tester les nonce premier 0 voir si ils sont à 0 si c'est bon on return true
    //si c'est pas bon on rappelle transforme avec nonce +1 tant que le nonce < limite
    //1 transforme le string en binaire
    string myString;
    string hash_str ;
    int nb ;
    Hacheur hacheur = Hacheur();
    while(m_bloc.nonce < NONCE_MAX ){
        myString = m_bloc.ToString();// A decommenter lors de l'insertion de la fonction ToString()
        //creation d'un objet Hacheur
        hash_str = hacheur.hacher(myString);// du hasheur
        //construnction du string de la taille du nombre des 0 du bloc hashé
        nb = retourNb0(hash_str);
        if(nb == m_nombreZeros){
            //affectation dans le bloc
            for(int i = 0 ; i <HASH_SIZE;i++ ){
                m_bloc.hash[i]=hash_str[i];
            }
            return true;
        }
        m_bloc.nonce++;
    }
   
    return false;

}

Bloc Mineur::getM_bloc() {
    return m_bloc;
}
