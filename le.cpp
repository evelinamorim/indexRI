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
#include <ctime>

timespec diff(timespec start, timespec end);

using namespace std;

/*Metodos da classe principal Le*/

Le::Le(string narquivo){
    conta_bits = 0;
    nome_arquivo = narquivo; 
    inicia_tamanho_arquivo();
}

void Le::inicia_tamanho_arquivo(){
    ifstream arquivo(nome_arquivo,ios::in|ios::binary|ios::ate);
    if (arquivo.is_open()){
	tamanho_arquivo = arquivo.tellg();
	cout << "TAMANHO_ARQUIVO: "<<tamanho_arquivo<<endl;
	arquivo.close();
    }else{
	cout << "Ordena::carrega_run::Nao foi possivel abrir o arquivo."<< endl;
    }
}

void Le::inicia_nome_arquivo(string narquivo){
    nome_arquivo = narquivo;
}

unsigned long int Le::pega_conta_bits(){
    return conta_bits;
}

void Le::inicia_conta_bits(unsigned int cb){
    conta_bits = cb;
}

int Le::ler_tripla_pos(deque<unsigned int>& v,int pos){
    //esta funcao le tripla ateh a posicao de bit dada
    ifstream arquivo(nome_arquivo,ios::in|ios::binary);
    int pos_arquivo;

    if (arquivo.is_open()){

        pos_arquivo = floor(conta_bits/32);
	int nnum = (pos-conta_bits)>32?(pos-conta_bits):32;

	arquivo.seekg(sizeof(int)*pos_arquivo,ios::beg);
	carrega_buffer(arquivo,nnum);

        while(conta_bits < pos){
	    v.push_back(ler_numero());


	    if (arquivo.eof() && buffer[0]==0)
	       pos_arquivo = -1;

	    pos_arquivo = arquivo.tellg();
	    if (buffer.size() == 0){
		break;
	    }
	    if (pos_arquivo<0 && buffer[0]!=0)
	       pos_arquivo = 1;


	    if (pos_arquivo<0) break;
	}


	arquivo.close();
    }else{
	cout << "Ler::ler_tripla_pos::Unable to open file." << endl;
    }

    return pos_arquivo;
}

int Le::ler_tripla(deque<unsigned int>& v,int nnum){

    //esta funcao le apenas uma tripla, dada a posicao atual do arquivo
    //depois atualiza pos com a nova posicao
    ifstream arquivo(nome_arquivo,ios::in|ios::binary);
    int terminou = 1;
    unsigned long int pos_arquivo;
    streampos parquivo;
    clock_t t;


    if (arquivo.is_open()){

        pos_arquivo = floor(conta_bits/32);
	//cout << "Conta_bits_leitura: " << conta_bits << endl;
	arquivo.seekg(sizeof(int)*pos_arquivo,ios::beg);

	t = clock();
	carrega_buffer(arquivo,nnum);
	t = clock() - t;
	//cout << "Carregar o buffer de leitura levou " << ((float)t/CLOCKS_PER_SEC) << "s" << endl; 

        for(int i=0;i<nnum;i++){
	    //break
	    v.push_back(ler_numero());
	    parquivo = arquivo.tellg();
	     if (buffer.size() != 0){
		//buffer zero nem sempre eh condicao de parada. Pode ter o numero 1 em unario la
		//TODO: Nao da final de lida por algum motivo
	        if ((parquivo == tamanho_arquivo || arquivo.eof())){
		    // cout << "Leitura: " << i << " "<< buffer[0] << " ";
		     if ( buffer[0] == 0){
			 if (buffer.size() > 1){
                            // cout << buffer[1] ;
			     //pode ter mais bits para frente
		             if (buffer[1] ==0){
				 //se nao houver bits mais para frente acabou mesmo
				 terminou = -1;
			     } else terminou = 1;
			 }else{
				 terminou = -1;
			 }   
		     }else terminou = 1;
		     //cout << endl;
		}
	    }else{
	        if ((parquivo == tamanho_arquivo)  || arquivo.eof()){
		    terminou = -1;}
	        else{
		    parquivo = arquivo.tellg();
		    terminou = 1;
		} 
	    }

	    if (terminou<0) break;
	}

        
	arquivo.close();
    }else{
	cout << "Ler::ler_tripla::Unable to open file." << endl;
    }

    return terminou;

}


/*Aqui estao os metodos da classe que faz leitura de inteiros em arquivo NAO compactado*/
LeNormal::LeNormal(string narquivo):Le(narquivo){}

LeNormal::~LeNormal(){}

void LeNormal::carrega_buffer(ifstream& arquivo,int nnum){
    //carrega buffer com os numeros a serem decodificados



    deque<unsigned int> tmp;
    tmp.swap(buffer);

    unsigned int *buf = new unsigned int[nnum+1]();
    streampos parquivo;
    int tam_buffer = nnum+1;

    for(int i=0;i<nnum+1;i++){
       arquivo.read((char*) &buf[i],sizeof(int));
       if (arquivo.eof()){
	   tam_buffer = i;
	   break;
       }
    }


    for(int i=0;i<tam_buffer;i++){
       buffer.push_back(buf[i]);
    }

    delete[] buf;
}

int LeNormal::ler_numero(){

    int numero = buffer.front();
    buffer.pop_front();

    conta_bits += 32;

    return numero;
}

/* Aqui estao os metodos da classe que faz leitura de inteiros em arquivo compactado */

LeCompacta::LeCompacta(string narquivo):Le(narquivo){}

LeCompacta::~LeCompacta(){}

void LeCompacta::carrega_buffer(ifstream& arquivo,int nnum){
    //carrega buffer com os numeros a serem decodificados

    deque<unsigned int> tmp;
    tmp.swap(buffer);


    unsigned int *buf = new unsigned int[nnum+1]();
    streampos parquivo;
    int tam_buffer = nnum+1;

    parquivo = arquivo.tellg();
    streampos buffer_bytes = nnum*sizeof(int);

    if ( (parquivo+buffer_bytes)>tamanho_arquivo){
       tam_buffer = (tamanho_arquivo-parquivo)/sizeof(int);
       arquivo.read((char*) buf,tam_buffer*sizeof(int));
    }else{ 
       arquivo.read((char*) buf,sizeof(int)*(nnum+1));
    }
    //bits ja lidos devem ser zerados. Devem existir pos_bit's para zerar
    int pos_bit = conta_bits % 32;

    for(int i=31;i>(31-pos_bit);i--){
	 buf[0] &= ~(1 << i);
    }

    for(int i=0;i<tam_buffer;i++){
       //cout << "reading buffer["<< i << "] = " << buf[i] << endl;
       buffer.push_back(buf[i]);
    }

    delete[] buf;
}

int LeCompacta::ler_numero(){


    //int pos_buffer = ceil(conta_bits/32);
    int pos_bit = conta_bits % 32;
    unsigned int nx;

    //int numero = gamma_para_int(buffer,nx,31-pos_bit);
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

