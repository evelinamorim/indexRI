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
int conta_numeros = 0;
//construtor
Escreve::Escreve(string narquivo){
    conta_bits = 0;
    conta_bits_global = 0;
    nome_arquivo = narquivo;
}



void Escreve::inicia_conta_bits(unsigned long int cb){
    conta_bits = cb;
}

unsigned long int Escreve::pega_conta_bits(){
    return conta_bits;
}

unsigned long int Escreve::pega_conta_bits_global(){
    return conta_bits_global;
}

string Escreve::pega_nome_arquivo(){
    return nome_arquivo;
}

streampos Escreve::escreve_tripla(vector<unsigned int> v){
    
    ofstream arquivo (nome_arquivo, ios::out|ios::binary|ios::app);
    
    streampos pos_arquivo;
    
    if (arquivo.is_open()){
        
        int tamv = v.size();
        
        carrega_buffer(tamv);
        
        vector<unsigned int>::iterator it;
        
        for(it=v.begin();it!=v.end();it++){
            escreve_numero(*it,2*tamv);
	    conta_numeros++;
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

    unsigned int long y=0;
    unsigned int long um = 1;
    para_codigo_gamma(x,y,ny);


    conta_bits_global += ny;
    
    
    //posicao no buffer que o numero deve ficar
    int i = floor(conta_bits /32);
    
    //posicao dentro do "bucket" do buffer para escrever
    int j = conta_bits % 32;

    //if ((32-j) < ny){
    if ((32-j) < ny){
        //quantidade de bits para escrever eh maior que a quantidade
        //disponivel no bucket atuala
	
	//e se ny for maior que 32 bits?Assumindo que nao havera mais 
	//que 64 bits
	int deslocamento;
        deslocamento = ny - (32-j);
        
	//OK
        for(int k=0;k<(32-j);k++) buffer[i] &= ~(1<<k);
        buffer[i] |= (y>>(deslocamento));
        
	//apagar em y quem foi para o buffer 0
        //for(int k=ny;k>(deslocamento);k--) y &= ~(um<<k);
        for(int k=ny;k>=(deslocamento);k--) y &= ~(um<<k);
	    

        buffer[i+1] = 0;
	if (deslocamento<=32){
           for(int k=31;k>(32-deslocamento);k--) buffer[i+1] &= ~(1<<k);
           buffer[i+1] |= (y << (32-deslocamento));
	}else{
           buffer[i+1] |= y >> (deslocamento-32);

	   //apagar em y quem foi para o buffer 1
           for(int k=deslocamento;k>(deslocamento-32);k--){
	       y &= ~(um<<k);
	   }

	   //quantos bits sobraram para o ultimo bucket?
	   int resto = deslocamento-32;
	   buffer[i+2] |= y << (32-resto);
	}
        conta_bits += ny;
    }else{
        int deslocamento = (32-j)-ny;
        for(int k=(31-j);k>=deslocamento;k--) buffer[i] &= ~(1<<k);
        buffer[i] |= (y << deslocamento);
        conta_bits += ny;
    }
}

void EscreveCompacta::carrega_buffer(int tamv){
    buffer = new unsigned int[(2*tamv)+1]();
    /*for(int i=0;i<(2*tamv)+1;i++){
        buffer[i]=0;
    }*/ 
    
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
