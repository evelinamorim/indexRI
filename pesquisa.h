/*
 * =====================================================================================
 *
 *       Filename:  pesquisa.h
 *
 *    Description:  arquivo de header de pesquisa
 *
 *        Version:  1.0
 *        Created:  02/25/14 22:51:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>

#include "colecao.h"
#include "le.h"


using namespace std;

class Pesquisa{
    static const string nome_arquivo_vocabulario;
    static const string nome_arquivo_indice;
    Colecao col;
    vector<int> posicoes_palavras;
    Le leitura;
    public:
    Pesquisa();
};
