/*
 * =====================================================================================
 *
 *       Filename:  ordena.cpp
 *
 *    Description:  esta classe tem o papel de ordenar um arquivo de indice em disco
 *
 *        Version:  1.0
 *        Created:  02/22/14 00:19:52
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#define MIN(a,b)  ({ __typeof__ (a) _a = (a);  __typeof__ (b) _b = (b); _a < _b ? _a : _b; })

#include "ordena.h"

#include <iostream>
#include <cstdlib>
#include <climits>
#include <cstdio>
#include <unordered_map>

using namespace std;

Ordena::Ordena(string narquivo):leitura(narquivo),escrita(narquivo + "tmp"),escrita_ordenada(narquivo+".ord"){
    nome_arquivo = narquivo;
    run = new tripla_t[TAMANHO_RUN];

    ifstream arquivo(nome_arquivo,ios::in|ios::binary|ios::ate);

    if (arquivo.is_open()){
	tamanho_arquivo = arquivo.tellg();
	arquivo.close();
    }else{
	cout << "Ordena::carrega_run::Nao foi possivel abrir o arquivo.";
    }

    conta_triplas = 0;
}

int Ordena::carrega_run(int& pos_arquivo){

    conta_bits.push_back(leitura.pega_conta_bits());
    vector<unsigned int> v;

    int i;
   // cout << "RUN" << endl;
    for(i=0;i<TAMANHO_RUN;i++){
	pos_arquivo = leitura.ler_tripla(v,3);
	run[i].pos = v.back();
	v.pop_back();
	run[i].doc = v.back();
	v.pop_back();
	run[i].lex = v.back();
	v.pop_back();
	conta_triplas++;
	//cout << run[i].lex << " " << run[i].doc << " " << run[i].pos << endl;
	if (pos_arquivo<0){ 
	    i = i+1;
	    break;
	}
    }
    //cout << endl;

    return i;
}

int Ordena::compara_triplas(tripla_t a,tripla_t b){

    if (a.lex < b.lex) return -1;
    if (a.lex > b.lex) return 1;
    if (a.lex == b.lex){
	if (a.doc < b.doc) return -1;
	if (a.doc > b.doc) return 1;
	return 0;
    }
    return 0;
}

void Ordena::merge_run(int i,int m,int f){
    int n1 = m-i;
    int n2 = f-m+1;

    tripla_t *esq,*dir;

    esq = new tripla_t[n1+1];
    dir = new tripla_t[n2+1];

    for(int k=0;k<n1;k++){
	esq[k].lex = run[i+k].lex;
	esq[k].doc = run[i+k].doc;
	esq[k].pos = run[i+k].pos;
    }

    esq[n1].lex = UINT_MAX;
    esq[n1].doc = UINT_MAX;
    esq[n1].pos = UINT_MAX;

    for(int k=0;k<n2;k++){
	dir[k].lex = run[m+k].lex;
	dir[k].doc = run[m+k].doc;
	dir[k].pos = run[m+k].pos;
    }

    dir[n2].lex = UINT_MAX;
    dir[n2].doc = UINT_MAX;
    dir[n2].pos = UINT_MAX;

    int j,l;
    j = l = 0;
    for(int k=i;k<=f;k++){
	if (compara_triplas(esq[j],dir[l])<=0){
	    run[k].lex = esq[j].lex;
	    run[k].doc = esq[j].doc;
	    run[k].pos = esq[j].pos;
	    j++;
	}else{
	    run[k].lex = dir[l].lex;
	    run[k].doc = dir[l].doc;
	    run[k].pos = dir[l].pos;
	    l++;
	}
    } 
    delete[] dir;
    delete[] esq;

}

void Ordena::ordena_run(int tam_run){
    //mergesort
    int m = 1;
    while(m<tam_run){
	int i = 0;
	while (i<(tam_run-m)){
	    merge_run(i,i+m,MIN(i+2*m-1,tam_run-1));
	    i = i + 2*m;
	}
	m = m*2;
    }
} 

 int Ordena::escreve_run(int tam_run){
    int inicio = conta_bits.back();
    int conta_bits_geral = inicio;
    int pos_arquivo;
    vector<unsigned int> v;
    //escrita.inicia_conta_bits(inicio);

   for(int i=0;i<tam_run;i++){

       v.push_back(run[i].lex);
       v.push_back(run[i].doc);
       v.push_back(run[i].pos);
       pos_arquivo = escrita.escreve_tripla(v);
       v.clear();
       if (pos_arquivo<0) break;
       
   }

    return pos_arquivo;
}

void Ordena::ordena_todas_runs(){

     int pos_arquivo = 0;
     while (pos_arquivo >= 0){
         int tam_run = carrega_run(pos_arquivo);
	 ordena_run(tam_run);
	 escreve_run(tam_run);
	 //cout << "POS_ARQUIVO: " << pos_arquivo << endl;
     }
    if (escrita.pega_excedente()!=0)
      escrita.escreve_excedente();


}

void Ordena::executa(unordered_map<string,int> vocabulario){
//void Ordena::executa(){
    

    //executa a ordenacao do indice em disco e atualiza 
    //a posicao da lista de termos em disco
    
    ordena_todas_runs();

    leitura.inicia_nome_arquivo(escrita.pega_nome_arquivo());

    tripla_t min,atual;
    vector<unsigned int> v;
    vector<int> limites(conta_bits);
    int min_conta_bits = 0;

    int num_conta_bits = conta_bits.size();


    int ntriplas = 0;
    int atualiza_conta_bits = 0;
    int termino_arquivo = 1;

    while(ntriplas<conta_triplas){
         min.lex = UINT_MAX;
         min.doc = UINT_MAX;
         min.pos = UINT_MAX;
	 v.clear();

	//TODO: eu posso eliminar algumas leituras de disco aqui.
	//mantendo em um buffer os as primeiras triplas e atualizando apenas a 
	//que mudar
        for(int i=0;i<num_conta_bits;i++){
	    //verificar se uma run nao 
	    //tem mais elemento
	    if (i!=(num_conta_bits-1)){
	       if (conta_bits[i]>=limites[i+1]) continue;
	    }else{
		if (conta_bits[i]<0) continue;
	    } 	
	
	    leitura.inicia_conta_bits(conta_bits[i]);

	    termino_arquivo = leitura.ler_tripla(v,3);
	    atual.pos = v.back();
	    v.pop_back();
	    atual.doc = v.back();
	    v.pop_back();
	    atual.lex = v.back();
	    v.pop_back();

	    if (compara_triplas(atual,min)<0){
		min.lex = atual.lex;
		min.doc = atual.doc;
		min.pos = atual.pos;
		min_conta_bits = i;
		if (termino_arquivo > 0)
		    atualiza_conta_bits = leitura.pega_conta_bits();
		else atualiza_conta_bits = -leitura.pega_conta_bits();
	    }
        }


	//vou assumir que o termo ja vai estar no mapeamento
	if (termos_pos[min.lex].size()==0){
	    //se a posicao do termo ainda nao foi marcada
	    //entao guardar esta posicao
	    if (conta_bits[i]>0)
	        termos_pos[min.lex].push_back(conta_bits[min_conta_bits]);
	    else termos_pos[min.lex].push_back(-conta_bits[min_conta_bits]);
	}
	conta_bits[min_conta_bits] = atualiza_conta_bits;
	//
	//cout << min.lex << " " << min.doc << " " << min.pos << endl;

	v.push_back(min.doc);
	v.push_back(min.pos);
    
        escrita_ordenada.escreve_tripla(v);

	ntriplas++;
    }

     //as runs ordenadas estao em um arquivo ordenado
     //entao vou pegar o conteudo do arquivo temporario e 
     //sobrecrever o arquivo original
     const char* narquivo = nome_arquivo.c_str();
     const char* narquivo_run = escrita.pega_nome_arquivo().c_str();
     const char* narquivo_ordenado = escrita_ordenada.pega_nome_arquivo().c_str();

     remove(narquivo_run);
     rename(narquivo_ordenado,narquivo);

}

/* int main(){
    //TODO: apagar arquivos temporarios
    Ordena o = Ordena("teste_comp4.bin");
    o.executa();
    return 0;
}*/



