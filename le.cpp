/*
 * =====================================================================================
 *
 *       Filename:  le.cpp
 *
 *    Description:  Le dados de um arquivo de indice invertido
 *
 *        Version:  1.0
 *        Created:  02/18/14 23:06:21
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "le.h"
#include "util.h"

using namespace std;

Le::Le(string narquivo){
    conta_bits = 0;
    nome_arquivo = narquivo; 
}

void Le::inicia_nome_arquivo(string narquivo){
    nome_arquivo = narquivo;
}

int Le::pega_conta_bits(){
    return conta_bits;
}

void Le::inicia_conta_bits(int cb){
    conta_bits = cb;
}

int Le::ler_tripla_pos(vector<unsigned int>& v,int pos){
    //esta funcao le tripla ateh a posicao de bit dada
    ifstream arquivo(nome_arquivo,ios::in|ios::binary);
    int pos_arquivo;

    if (arquivo.is_open()){

        pos_arquivo = floor(conta_bits/32);
	int nnum = (pos-conta_bits)>32?(pos-conta_bits):32;
	arquivo.seekg(sizeof(int)*pos_arquivo,ios::beg);
	carrega_buffer(arquivo,nnum);

	//TODO: acho que pode ler um pouco mais do que deveria aqui
        while(conta_bits < pos){
	    v.push_back(ler_numero());
	    if (arquivo.eof() && buffer[0]==0)
	       pos_arquivo = -1;

	    pos_arquivo = arquivo.tellg();

	    if (pos_arquivo<0 && buffer[0]!=0)
	       pos_arquivo = 1;

	    if (pos_arquivo<0) break;
	}




	arquivo.close();
    }else{
	cout << "Ler::ler_tripla::Unable to open file." << endl;
    }

    return pos_arquivo;
}

int Le::ler_tripla(vector<unsigned int>& v,int nnum){

    //esta funcao le apenas uma tripla, dada a posicao atual do arquivo
    //depois atualiza pos com a nova posicao
    ifstream arquivo(nome_arquivo,ios::in|ios::binary);
    int pos_arquivo;

    if (arquivo.is_open()){

        pos_arquivo = floor(conta_bits/32);
	arquivo.seekg(sizeof(int)*pos_arquivo,ios::beg);
	carrega_buffer(arquivo,nnum);

        for(int i=0;i<nnum;i++){
	    v.push_back(ler_numero());
	}


	if (arquivo.eof() && buffer[0]==0)
	    pos_arquivo = -1;

	pos_arquivo = arquivo.tellg();

	if (pos_arquivo<0 && buffer[0]!=0)
	    pos_arquivo = 1;

	arquivo.close();
    }else{
	cout << "Ler::ler_tripla::Unable to open file." << endl;
    }

    return pos_arquivo;

}

void Le::carrega_buffer(ifstream& arquivo,int nnum){
    //carrega buffer com os numeros a serem decodificados

    buffer.clear();

    unsigned int *buf = new unsigned int[nnum];

    for(int i=0;i<nnum;i++)
       arquivo.read((char*) &buf[i],sizeof(int));

    //bits ja lidos devem ser zerados. Devem existir pos_bit's para zerar
    int pos_bit = conta_bits % 32;

    for(int i=31;i>(31-pos_bit);i--){
	 buf[0] &= ~(1 << i);
    }


    for(int i=0;i<nnum;i++)
       buffer.push_back(buf[i]);

    delete[] buf;
}

int Le::ler_numero(){

    //int pos_buffer = ceil(conta_bits/32);
    int pos_bit = conta_bits % 32;
    int nx;

    int numero = gamma_para_int(buffer,nx,31-pos_bit);

    conta_bits += nx;

    return numero;
}

/*int main(){
    Le l = Le("index.bin");
    unsigned int lex,doc,pos;
    vector<unsigned int> v;

    //1
    for(int i = 0;i< 13650;i++){
    l.ler_tripla(v,3);

    pos = v.back();
    v.pop_back();
    doc = v.back();
    v.pop_back();
    lex = v.back();
    v.pop_back();
    cout << lex<< " " << doc << " " << pos << endl;}



    return 0;
}*/

