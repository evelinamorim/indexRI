/*
 * =====================================================================================
 *
 *       Filename:  util.cpp
 *
 *    Description:  funcoes uteis para este trabalho
 *
 *        Version:  1.0
 *        Created:  02/10/14 14:19:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Evelin Amorim, 
 *   Organization:  
 *
 * =====================================================================================
 */


#include "util.h"


void converteParaMinusculo(string& s){
    /* COnverte uma string para minusculo */
    transform(s.begin(), s.end(), s.begin(), ::tolower);
}

bool ehPontuacao(char c){
    return (c == ';' || c == ':' || c == '.' || c == '?' || c == ',' || c == '!' || c==')' || c=='(' || c == '\n' || c == '\t' || c == '\r' || c == '-');
}
vector<string> tokenizar(string s){
    string::iterator it = s.begin();
    vector<string> v;

    string palavra = "";

    while(it!=s.end()){
	if (*it == ' ' || ehPontuacao(*it)){
	    if (palavra.size() > 0) v.push_back(palavra);
	    palavra = "";
	}else{
	    palavra += *it;
	}
	it++;
    }

    return v;
}
