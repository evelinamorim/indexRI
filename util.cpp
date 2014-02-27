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

#include <iostream>
#include <cstdlib>
#include "util.h"

using namespace std;

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

    ny = ny + (2*lpiso) + 1;
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

unsigned int unario_para_int(vector<unsigned int>& x,int pos){
    //pegar unario a partir da posicao pos

    unsigned int cbits = 0;
    bool naoterminado = true;

    int i=pos;
    for(;i>=0;i--){
	 if ((x.front() & (1 << i))!=0){
	     cbits++;
	     if (i==0 && naoterminado){
		 x.erase(x.begin());
		 i=32;
	     }
	 }else{
	    naoterminado = false;
            break;
	 } 
    }

    return cbits+1;
}

unsigned int gamma_para_int(vector<unsigned int>& x,int& nx,int pos){

    //transformar de gamma para int a partir do bit pos

    unsigned int cu;
    int tamx = x.size();

    cu = unario_para_int(x,pos);


    nx = 2*cu-1;

    unsigned int y = x.front();
    int cbits;
    cbits = 0;
    int resto = cu;
    int deslocamento = pos-(2*cu-1)+1;
    int pos_cb = pos+cu;//essa inicializacao eh valida quando cb e cu estao 
    //no mesmo bit

    //zerando a parte unaria

    //neste caso a parte unaria esta dividida entre dois 
    //inteiros, entao a posicao do numero lido atualmente volta 
    //para o bit numero 31
    if (tamx != x.size()){ 
	resto = cu-pos-1;
	//pos+1: quantidade de bits na parte 1
	//cu+1+resto: quantidade de bits na parte 2
	deslocamento = 32-(cu-1+resto);
	//resto = cu-1+resto;
	pos = 31;
	pos_cb = pos-resto;
    }

    for(int i=pos;i>(pos-resto);i--){
	 if ((x.front() & (1 << i))!=0){
	     y &= ~(1 << i);
	     cbits++;
	 }else{
	     if (cbits!=0){
		 pos_cb=i-1;
		 break;
	     } 
	 }
    }

    unsigned int cb;
    //cout << "Deslocamento: " << deslocamento << endl;

    //Neste caso a parte binaria esta dividida em duas partes
    int qtd_bits_cb = cu-1;
    if (((pos_cb+1)-qtd_bits_cb)<0){
	int qtd_bits_dir = fabs((pos_cb+1)-((int)cu-1));
	qtd_bits_cb = qtd_bits_dir;

        x.erase(x.begin());
	//a primeira parte deste ou logico pega a primeira parte da parte
	//binaria, e a segunda parte deve pegar a parte posterior do numero
	//que acabou ficando em outro inteiro. Fazendo o ou logico
	//consigo parte binaria
	//cout << "1: " <<  y << " - "<< (y << (qtd_bits_dir)) << endl;
	//cout << "2: " <<  x.front() << " - " <<  (x.front() >> 30) << endl;
	cb = (y << (qtd_bits_dir)) | (x.front() >> (32-qtd_bits_dir));
	pos_cb = 31;
	y = x.front();
    }
    else cb = y >> (deslocamento);

    unsigned int numeroint = pow(2,cu-1) + cb;

    //zerando a parte binaria
    for(int i=pos_cb;i>(pos_cb-qtd_bits_cb);i--) y &= ~(1 << i);

   // cout << " 1: " << y << endl << endl;

    //verificando se acabou o buffer sem precisar particionar
    if ((pos+1)!=nx) x[0] = y;
    else x.erase(x.begin());
    return numeroint;
}
