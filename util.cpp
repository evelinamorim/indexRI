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


bool arquivoExiste(const string nomeArquivo){
    struct stat buf;

    if (stat(nomeArquivo.c_str(),&buf) != 1) return true;
    return false;
}

void para_codigo_gamma(unsigned int x,unsigned int& y,unsigned int& ny){
    ny = 0;
    int lpiso = floor(log2(x));

    ny += ny + (2*lpiso) + 1;
    int cunario = para_codigo_unario(lpiso+1);
    int pbinaria = x-pow(2,lpiso);

    y = cunario;
    y = y << lpiso;
    y |= pbinaria;
}

int para_codigo_unario(int x){
    //dado um numero inteiro escreve um codigo unario
    int y = 0;
    for(int i=1;i<x;i++){
	y |= (1 << i);
    }
    return y;
}
