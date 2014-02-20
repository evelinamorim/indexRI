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

int Escreve::escreve_tripla(int lex,int doc,vector<unsigned int> v){

    //TODO: separar por <termo,doc,v[i]>
    ofstream arquivo (nome_arquivo, ios::out|ios::binary|ios::app);
    int tam_lista = 0;//o tamanho da lista no arquivo


    if (arquivo.is_open()){

	int tamv = v.size();
        carrega_buffer(tamv);

	vector<unsigned int>::iterator it;

	for(it=v.begin();it!=v.end();it++){
	    escreve_numero(lex);
	    escreve_numero(doc);
	    escreve_numero(*it);
	}

	escreve_buffer(arquivo);

	tam_lista = arquivo.tellp();

        arquivo.close();
    }else{
	cout << "Colecao::escreve_tripla::Unable to open file" << endl;
    }
    return tam_lista;
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

    if (conta_bits % 32 !=0 ){
	excedente = buffer[nbytes];
	nbytes = nbytes -1;
	conta_bits = conta_bits % 32;
    }else{
        conta_bits = 0; 
	excedente = 0;
    }	
	
    arquivo.write((char*) buffer,nbytes);
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


