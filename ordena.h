/*
 * =====================================================================================
 *
 *       Filename:  ordena.h
 *
 *    Description:  arquivo de header da classe que ordena o arquivo intermediario
 *    de indices. Tambem escreve o arquivo final de indices
 *
 *        Version:  1.0
 *        Created:  02/22/14 15:17:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

//TODO: depois mudar este tamanho. No momento eh soh para teste
#define TAMANHO_RUN 5

#include "le.h"
#include "escreve.h"
#include "colecao.h"

#include <unordered_map>

using namespace std;

#ifndef __ORDENA_H_
#define __ORDENA_H_

typedef struct tripla{
    unsigned int lex;
    unsigned int doc;
    unsigned int pos;
}tripla_t;

class Ordena{
    Le* leitura;
    Escreve* escrita;
    Escreve* escrita_ordenada;
    tripla_t* run;
    string nome_arquivo;
    //este vetor eh para marcar a posicao de inicio de cada run
    vector<int> conta_bits;
    int conta_triplas;
    streampos tamanho_arquivo;

    tripla_t *buffer_ordenacao;

    public:
    Ordena(string narquivo,bool compacta);

    int carrega_run(int& pos_arquivo);
    void ordena_run(int tam_run);
    int escreve_run(int tam_run);
    void ordena_todas_runs();
    int compara_triplas(tripla_t a,tripla_t b);
    void merge_run(int i,int m,int f);
    void executa(Colecao& col);
    void atualiza_buffer_ordenacao(int pos,int* pos_prox);
    void carrega_buffer_ordenacao(int* pos_prox);
};

#endif