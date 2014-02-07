/*
 * =====================================================================================
 *
 *       Filename:  index.cpp
 *
 *    Description:  Deveria ser o arquivo principal para o indice
 *
 *        Version:  1.0
 *        Created:  02/06/14 23:29:06
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Evelin Amorim, 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "colecao.h"
#include "CollectionReader.h"

#include <cstdlib>
#include <iostream>


using namespace std;
using namespace RICPNS;


int main(int argc,char** argv){
    string dirEntrada = argv[1];
    string nomeIndice = argv[2];

    Colecao *col = new Colecao();

    col->ler(dirEntrada,nomeIndice);

    return 0;
}
