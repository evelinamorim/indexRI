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

/* Metodos da classe Escreve */

//construtor
Escreve::Escreve(string narquivo){
    conta_bits = 0;
    conta_bits_global = 0;
    nome_arquivo = narquivo;
}



void Escreve::inicia_conta_bits(unsigned int cb){
    conta_bits = cb;
}

unsigned int Escreve::pega_conta_bits(){
    return conta_bits;
}

unsigned int Escreve::pega_conta_bits_global(){
    return conta_bits_global;
}

string Escreve::pega_nome_arquivo(){
    return nome_arquivo;
}

int Escreve::escreve_tripla(vector<unsigned int> v){

    ofstream arquivo (nome_arquivo, ios::out|ios::binary|ios::app);

    int pos_arquivo;

    if (arquivo.is_open()){

	int tamv = v.size();

        carrega_buffer(tamv);

	vector<unsigned int>::iterator it;


	for(it=v.begin();it!=v.end();it++){
	    escreve_numero(*it,2*tamv);
	}

	escreve_buffer(arquivo,2*tamv);

	pos_arquivo = arquivo.tellp();

        arquivo.close();
    }else{
	cout << "Colecao::escreve_tripla::Unable to open file" << endl;
    }
    return pos_arquivo;
}

/* Metodos da classe EscreveNormal*/

EscreveNormal::EscreveNormal(string narquivo): Escreve(narquivo){}

EscreveNormal::~EscreveNormal(){}

void EscreveNormal::escreve_numero(unsigned int x,int tam_buffer){
    conta_bits_global += 32;

    //posicao no buffer que o numero deve ficar
    int i = floor(conta_bits /32);

    buffer[i] = x;
    conta_bits += 32;
}

void EscreveNormal::carrega_buffer(int tamv){
    buffer = new unsigned int[tamv]();
}

void EscreveNormal::escreve_buffer(ofstream& arquivo,int tam_buffer){
    //quantidade de bytes que esta no buffer
    int nint = ceil(conta_bits/32.0);

    arquivo.write((char*) buffer,nint*sizeof(int));
    conta_bits = 0;
    delete[] buffer;
}

/* Metodos da classe EscreveCompacta */

EscreveCompacta::EscreveCompacta(string narquivo): Escreve(narquivo){
    excedente = new unsigned int[1];
    *excedente = 0;
}

EscreveCompacta::~EscreveCompacta()
{
   delete[] excedente;
}

void EscreveCompacta::inicia_excedente(unsigned int e){
    *excedente = e;
}

unsigned int EscreveCompacta::pega_excedente(){
    return *excedente;
}

void EscreveCompacta::escreve_numero(unsigned int x,int tam_buffer){
    //guarda numero em um buffer
    unsigned int ny = 0;
    unsigned int y;
    para_codigo_gamma(x,y,ny);
    conta_bits_global += ny;


    //posicao no buffer que o numero deve ficar
    int i = floor(conta_bits /32);

    //posicao dentro do "bucket" do buffer para escrever
    int j = conta_bits % 32;

    //if ((32-j) < ny){
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

void EscreveCompacta::carrega_buffer(int tamv){
    buffer = new unsigned int[(2*tamv)+1]();
    for(int i=0;i<(2*tamv)+1;i++){
       buffer[i]=0;
    }	    
    
    //aqui deve carregar bits que sobraram em alguma escrita anterior
    if (conta_bits !=0){
	buffer[0] = 0;
	buffer[0] |= *excedente;
	*excedente = 0;
    }	
}

void EscreveCompacta::escreve_buffer(ofstream& arquivo,int tam_buffer){
    //quantidade de bytes que esta no buffer que devo escrever
    int nbytes = ceil(conta_bits/8);
    int nint = floor(conta_bits/32.0);

 /* for (int i = 0;i< nint;i++){
	cout << "write buffer[" << i << "] = " <<buffer[i] << endl;
    }*/ 

    //se houver excedente, deixa como excedente que a proxima escrita resolve
    if (conta_bits % 32 !=0 ){
	if (nint > tam_buffer) cout << "UAU: " << nint << endl;
	*excedente = buffer[nint];
	conta_bits = conta_bits % 32;
    }else{
        conta_bits = 0; 
	*excedente = 0;
    }	
    arquivo.write((char*) buffer,nint*sizeof(int));
    delete[] buffer;
}

void EscreveCompacta::escreve_excedente(){
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


