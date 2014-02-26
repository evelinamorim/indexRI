/*
 * =====================================================================================
 *
 *       Filename:  documento.h
 *
 *    Description:  com as interfaces de classes para documento.cpp
 *
 *        Version:  1.0
 *        Created:  02/06/14 23:13:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Evelin Amorim, 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "escreve.h"

#include <vector>
#include <htmlcxx/html/ParserDom.h>
#include <unordered_map>
#include <string>
#include <fstream>

using namespace std;

#ifndef __COLECAO_H_
#define __COLECAO_H_

class Colecao{
    //lista de documentos
    //TODO: nao carregar tudo na memoria?
    //vector<Documento> lista;

    unordered_map<string,int> vocabulario;
    unordered_map<int,string> vocabulario_invertido;

    int contaPalavras;
    static const string nome_arquivo_vocabulario;
    static const string nome_arquivo_indice;
    Escreve escrita;

    public:
    Colecao();
    void ler(string dirEntrada,string nomeIndice);
    unordered_map<int,vector<int> > ler_arvore_dom(tree<htmlcxx::HTML::Node> arvoreDom,int doc);
    vector<string> lerLexicais(string sentenca);
    void armazena_termos_doc(unordered_map<int,vector<int> > termos_pos,int doc);
    const string pega_nome_arquivo_indice();
    int pega_tamanho_vocabulario();

    void atualiza_vocabulario(int lex,int pos);
    void escreve_vocabulario();

    //TODO: fazer ler vocabulario

};

#endif
