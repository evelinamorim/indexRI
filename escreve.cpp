/*
 * =====================================================================================
 *
 *       Filename:  escreve.cpp
 *
 *    Description:  tem a classe que escreve em disco uma lista de um dados lexico
 *
 *        Version:  1.0
 *        Created:  02/17/14 18:46:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#include "escreve.h"
#include "util.h"

using namespace std;

Escreve::Escreve(string narquivo){
    conta_bits = 0;
    nome_arquivo = narquivo;
}

void Escreve::escreve_lista(int lex,int doc,vector<unsigned int> v){
    ofstream arquivo (nome_arquivo, ios::out|ios::binary|ios::app);

    if (arquivo.is_open()){

	//TODO: compactar lex, doc e tamv?
	arquivo.write((char*) &lex,sizeof(int));
	arquivo.write((char*) &doc,sizeof(int));

	int tamv = v.size();

	buffer = new unsigned int[tamv];
	arquivo.write((char*) &tamv,sizeof(int));
	vector<unsigned int>::iterator it;

	for(it=v.begin();it!=v.end();it++){
	    escreve_numero(*it);
	}

	escreve_buffer(arquivo);

        arquivo.close();
	delete[] buffer;
    }else{
	cout << "Colecao::escreve_lista::Unable to open file" << endl;
    }
}

void Escreve::escreve_numero(unsigned int x){
    //guarda numero em um buffer
    unsigned int ny,y;
    para_codigo_gamma(x,y,ny);


    //posicao no buffer que o numero deve ficar
    int i = floor(conta_bits /32);

    //posicao dentro do "bucket" do buffer para escrever
    int j = conta_bits % 32;

    if ((32-j) < ny){
	//quantidade de bits para escrever eh maior que a quantidade 
	//disponivel no bucket atual
	int deslocamento = ny - (32-j);
	buffer[i] = buffer[i] | (y>>deslocamento);
	buffer[i+1] = buffer[i+1] | (y << (32-deslocamento));
	conta_bits += ny;
    }else{
	int deslocamento = (32-j)-ny;
	buffer[i] = buffer[i] | (y << deslocamento);
	conta_bits += ny;
    }
}

void Escreve::escreve_buffer(ofstream& arquivo){
    //quantidade de bytes que esta no buffer que devo escrever
    int nbytes = ceil(conta_bits/8);
    arquivo.write((char*) buffer,nbytes);
}

int main(){
    vector<unsigned int> v;

    v.push_back(10);
    v.push_back(7);
    v.push_back(9);
    v.push_back(10);
    v.push_back(7);
    v.push_back(9);
    v.push_back(11);

    Escreve e = Escreve("teste_comp.bin");

    e.escreve_lista(1,1,v);


    return 0;
}


