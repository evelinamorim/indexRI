
/*
 * * =====================================================================================
 * *
 * * Filename: index.cpp
 * *
 * * Description: Deveria ser o arquivo principal para o indice
 * *
 * * Version: 1.0
 * * Created: 02/06/14 23:29:06
 * * Revision: none
 * * Compiler: gcc
 * *
 * * Author: Evelin Amorim,
 * * Organization:
 * *
 * * =====================================================================================
 * */

#include "colecao.h"
#include "ordena.h"
#include "CollectionReader.h"

#include <cstdlib>
#include <iostream>


using namespace std;
using namespace RICPNS;


int main(int argc,char** argv){
    string dirEntrada = argv[1];
    string nomeIndice = argv[2];

    Colecao *col = new Colecao();

    //le a colecao e controi o indice desordenado
    col->ler(dirEntrada,nomeIndice);

    Ordena *ordenar = new Ordena(col->pega_nome_arquivo_indice());

    ordenar->executa(*col);

    col->escreve_vocabulario();

    return 0;
}


