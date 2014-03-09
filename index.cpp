
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
#include <cstring>


using namespace std;
using namespace RICPNS;


int main(int argc,char** argv){
    string dirEntrada = argv[1];
    string nomeIndice = argv[2];

    bool compacta = false;

    if (argc == 4){
	if (strncmp(argv[3],"-c",2)==0) compacta = true;
    }


    Colecao *col = new Colecao(compacta);

    //le a colecao e controi o indice desordenado
   // col->ler(dirEntrada,nomeIndice);

   Ordena *ordenar = new Ordena(col->pega_nome_arquivo_indice(),compacta);

   cout << "Antes de orndenar" << endl;
    ordenar->executa(*col);

   col->escreve_vocabulario();

    delete col;
    delete ordenar;
    return 0;
}


