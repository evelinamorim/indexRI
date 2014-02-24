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
    excedente = new unsigned int[1];
    *excedente = 0;
}

void Escreve::inicia_conta_bits(int cb){
    conta_bits = cb;
}

int Escreve::pega_conta_bits(){
    return conta_bits;
}

void Escreve::inicia_excedente(unsigned int e){
    *excedente = e;
}

unsigned int Escreve::pega_excedente(){
    return *excedente;
}

string Escreve::pega_nome_arquivo(){
    return nome_arquivo;
}


int Escreve::escreve_tripla(vector<unsigned int> v){

    ofstream arquivo (nome_arquivo, ios::out|ios::binary|ios::app);

    int pos_arquivo;

    if (arquivo.is_open()){

	/* TODO:Acho que nao vai fazer diferenca, considerando 
	 que esta no modo append e sempre escrevera no fim do 
	 arquivo*/ 
	/*if (conta_bits!=0){
	    int pos_atual = floor(conta_bits/8);
	    arquivo.seekp(pos_atual,ios::beg);
	}else{
	    arquivo.seekp(0,ios::beg);
	}*/ 



	//somando mais dois para alocar espaco no  buffer para lex e para doc
	int tamv = v.size();

        carrega_buffer(tamv);

	vector<unsigned int>::iterator it;



	for(it=v.begin();it!=v.end();it++){
	    escreve_numero(*it);
	}

	// cout << "Numero de bits: " << conta_bits << endl;

	escreve_buffer(arquivo);

	pos_arquivo = arquivo.tellp();
	// cout << "Posicao arquivo: " << pos_arquivo << endl;


        arquivo.close();
    }else{
	cout << "Colecao::escreve_tripla::Unable to open file" << endl;
    }
    return pos_arquivo;
}

void Escreve::escreve_numero(unsigned int x){
    //guarda numero em um buffer
    unsigned int ny = 0;
    unsigned int y;
    para_codigo_gamma(x,y,ny);


    //posicao no buffer que o numero deve ficar
    int i = floor(conta_bits /32);

    //posicao dentro do "bucket" do buffer para escrever
    int j = conta_bits % 32;

    if ((32-j) < ny){
	//quantidade de bits para escrever eh maior que a quantidade 
	//disponivel no bucket atual
	int deslocamento = ny - (32-j);

	for(int k=0;k<(32-j);k++) buffer[i] &= ~(1<<k);
	buffer[i] |= (y>>(deslocamento));

	buffer[i+1] = 0;
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
    buffer = new unsigned int[tamv]();
    for(int i=0;i<tamv;i++){
       buffer[i]=0;
    }	    
    
    //aqui deve carregar bits que sobraram em alguma escrita anterior
    if (conta_bits !=0){
	buffer[0] = 0;
	buffer[0] |= *excedente;
	*excedente = 0;
    }	
}

void Escreve::escreve_buffer(ofstream& arquivo){
    //quantidade de bytes que esta no buffer que devo escrever
    int nbytes = ceil(conta_bits/8);
    int nint = floor(conta_bits/32.0);

	for (int i=0;i<nint;i++) 
	    cout << "buffer[" << i << "]: " << buffer[i] << endl;
    if (conta_bits % 32 !=0 ){
	*excedente = buffer[nint];
	conta_bits = conta_bits % 32;
    }else{
        conta_bits = 0; 
	*excedente = 0;
    }	
    arquivo.write((char*) buffer,nint*sizeof(int));
    delete[] buffer;
}

void Escreve::escreve_excedente(){
    ofstream arquivo (nome_arquivo, ios::out|ios::binary|ios::app);
    if (arquivo.is_open()){
	arquivo.seekp (arquivo.end);
        arquivo.write((char*) excedente,sizeof(int));
	arquivo.close();
    }else{
	cout << "Escreve::escreve_excedente::Nao foi possivel abrir arquivo." << endl;
    }
}

/* int main(){
    vector<unsigned int> v;

    v.push_back(10);
    v.push_back(7);
    v.push_back(9);
    v.push_back(10);
    v.push_back(7);
    v.push_back(9);
    v.push_back(11);
    v.push_back(23);

    vector<unsigned int> v1;

    v1.push_back(3);
    v1.push_back(2);
    v1.push_back(9);

    vector<unsigned int> v2;

    v2.push_back(31);
    v2.push_back(27);
    v2.push_back(96);

    vector<unsigned int> v3;

    v3.push_back(101);
    v3.push_back(102);
    v3.push_back(103);



    Escreve e = Escreve("teste_comp5.bin");

    e.escreve_tripla(1,1,v);
    e.escreve_tripla(1,2,v1);
    e.escreve_tripla(2,4,v2);
    e.escreve_tripla(100,100,v3);



    if (e.pega_excedente()!=0)
      e.escreve_excedente();


    return 0;
}*/


