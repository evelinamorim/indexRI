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

//TODO: mudar para argumento com vector. Assim fica generico. Como argumento
//colocar um vector que vai ser preenchido e quantos valores!
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

/* int main(){
    Le l = Le("teste_comp4.bintmp");
    unsigned int lex,doc,pos;
    vector<unsigned int> v;

    //1
    l.ler_tripla(v,3);

    pos = v.back();
    v.pop_back();
    doc = v.back();
    v.pop_back();
    lex = v.back();
    v.pop_back();
    cout << lex << " " << doc << " " << pos << endl;

    //2
    l.ler_tripla(v,3);

    pos = v.back();
    v.pop_back();
    doc = v.back();
    v.pop_back();
    lex = v.back();
    v.pop_back();
    cout << lex << " " << doc << " " << pos << endl;

    //3
    l.ler_tripla(v,3);

    pos = v.back();
    v.pop_back();
    doc = v.back();
    v.pop_back();
    lex = v.back();
    v.pop_back();
    cout << lex << " " << doc << " " << pos << endl;

    //4
    l.ler_tripla(v,3);

    pos = v.back();
    v.pop_back();
    doc = v.back();
    v.pop_back();
    lex = v.back();
    v.pop_back();
    cout << lex << " " << doc << " " << pos << endl;

    //5
    l.ler_tripla(v,3);

    pos = v.back();
    v.pop_back();
    doc = v.back();
    v.pop_back();
    lex = v.back();
    v.pop_back();
    cout << lex << " " << doc << " " << pos << endl;

    //6
    l.ler_tripla(v,3);

    pos = v.back();
    v.pop_back();
    doc = v.back();
    v.pop_back();
    lex = v.back();
    v.pop_back();
    cout << lex << " " << doc << " " << pos << endl;

    //7
    l.ler_tripla(v,3);

    pos = v.back();
    v.pop_back();
    doc = v.back();
    v.pop_back();
    lex = v.back();
    v.pop_back();
    cout << lex << " " << doc << " " << pos << endl;

    //8
    l.ler_tripla(v,3);

    pos = v.back();
    v.pop_back();
    doc = v.back();
    v.pop_back();
    lex = v.back();
    v.pop_back();
    cout << lex << " " << doc << " " << pos << endl;

    //9
    l.ler_tripla(v,3);

    pos = v.back();
    v.pop_back();
    doc = v.back();
    v.pop_back();
    lex = v.back();
    v.pop_back();
    cout << lex << " " << doc << " " << pos << endl;

    //10
    l.ler_tripla(v,3);

    pos = v.back();
    v.pop_back();
    doc = v.back();
    v.pop_back();
    lex = v.back();
    v.pop_back();
    cout << lex << " " << doc << " " << pos << endl;

    //11
    l.ler_tripla(v,3);

    pos = v.back();
    v.pop_back();
    doc = v.back();
    v.pop_back();
    lex = v.back();
    v.pop_back();
    cout << lex << " " << doc << " " << pos << endl;

    //12
    l.ler_tripla(v,3);

    pos = v.back();
    v.pop_back();
    doc = v.back();
    v.pop_back();
    lex = v.back();
    v.pop_back();
    cout << lex << " " << doc << " " << pos << endl;

    //13
    l.ler_tripla(v,3);

    pos = v.back();
    v.pop_back();
    doc = v.back();
    v.pop_back();
    lex = v.back();
    v.pop_back();
    cout << lex << " " << doc << " " << pos << endl;

    //14
    l.ler_tripla(v,3);

    pos = v.back();
    v.pop_back();
    doc = v.back();
    v.pop_back();
    lex = v.back();
    v.pop_back();
    cout << lex << " " << doc << " " << pos << endl;

    //15
    l.ler_tripla(v,3);

    pos = v.back();
    v.pop_back();
    doc = v.back();
    v.pop_back();
    lex = v.back();
    v.pop_back();
    cout << lex << " " << doc << " " << pos << endl;

    //16
    l.ler_tripla(v,3);

    pos = v.back();
    v.pop_back();
    doc = v.back();
    v.pop_back();
    lex = v.back();
    v.pop_back();
    cout << lex << " " << doc << " " << pos << endl;

    //17
    l.ler_tripla(v,3);

    pos = v.back();
    v.pop_back();
    doc = v.back();
    v.pop_back();
    lex = v.back();
    v.pop_back();
    cout << lex << " " << doc << " " << pos << endl;
    return 0;
}*/ 

