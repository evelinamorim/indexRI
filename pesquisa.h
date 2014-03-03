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
    vector<int> posicoes_palavras;
    Le* leitura;
    Colecao* col;

    public:
    Pesquisa(bool compact);
    unordered_map<unsigned int,vector<unsigned int> > executa(string palavra);
    void imprime_docs_resultados(unordered_map<unsigned int,vector<unsigned int> > resultado,string dir_entrada,string nome_indice);
    vector<string> processa_consulta(string consulta,int& tipo_consulta);
    void intersecao(unordered_map<unsigned int,vector<unsigned int> >& r1, unordered_map<unsigned int,vector<unsigned int> >& r2);
};
