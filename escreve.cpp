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

void Escreve::inicia_conta_bits(int cb){
    conta_bits = cb;
}

void Escreve::inicia_excedente(int e){
    excedente = e;
}


int Escreve::escreve_tripla(int lex,int doc,vector<unsigned int> v){

    ofstream arquivo (nome_arquivo, ios::out|ios::binary|ios::app);

    int pos_arquivo;

    if (arquivo.is_open()){

	if (conta_bits!=0){
	    int pos_atual = floor(conta_bits/8);
	    arquivo.seekp(pos_atual,ios::beg);
	}


	int tamv = v.size();
        carrega_buffer(tamv);

	vector<unsigned int>::iterator it;

	for(it=v.begin();it!=v.end();it++){
	    escreve_numero(lex);
	    escreve_numero(doc);
	    escreve_numero(*it);
	}

	escreve_buffer(arquivo);

	pos_arquivo = arquivo.tellp();

        arquivo.close();
    }else{
	cout << "Colecao::escreve_tripla::Unable to open file" << endl;
    }
    return pos_arquivo;
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

	for(int k=0;k<deslocamento;k++) buffer[i] &= ~(1<<k);
	buffer[i] |= (y>>deslocamento);

	for(int k=31;k<(32-deslocamento);k--) buffer[i+1] &= ~(1<<k);
	buffer[i+1] |= (y << (32-deslocamento));
	conta_bits += ny;
    }else{
	int deslocamento = (32-j)-ny;

	for(int k=(32-j);k<deslocamento;k--) buffer[i] &= ~(1<<k);
	buffer[i] |= (y << deslocamento);
	conta_bits += ny;
    }
}

void Escreve::carrega_buffer(int tamv){
    buffer = new unsigned int[tamv];

    //aqui deve carregar bits que sobraram em alguma escrita anterior
    if (conta_bits !=0){
	buffer[0] |= excedente;
    }	
}

void Escreve::escreve_buffer(ofstream& arquivo){
    //quantidade de bytes que esta no buffer que devo escrever
    int nbytes = ceil(conta_bits/8);
    int nint = ceil(conta_bits/32.0);

    if (conta_bits % 32 !=0 ){
	excedente = buffer[nbytes];
	nbytes = nbytes -1;
	conta_bits = conta_bits % 32;
    }else{
        conta_bits = 0; 
	excedente = 0;
    }	
    arquivo.write((char*) buffer,nint*sizeof(int));
    delete[] buffer;
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

    e.escreve_tripla(1,1,v);


    return 0;
}


