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

using namespace std;

class Colecao{
    //lista de documentos
    //TODO: nao carregar tudo na memoria?
    //vector<Documento> lista;

    
    unordered_map<string,int> vocabulario;
    int contaPalavras;
    const string nome_arquivo_vocabulario = "voc.txt"
    const string nomeArquivoIndice = "index.bin";
    Escreve escrita(nomeArquivoIndice);

    public:
    Colecao();
    void ler(string dirEntrada,string nomeIndice);
    unordered_map<int,vector<int>> ler_arvore_dom(tree<htmlcxx::HTML::Node> arvoreDom);
    vector<string> lerLexicais(string sentenca);
    void Colecao::armazena_termos_doc(unordered_map<int,vector<int>> termos_pos,int doc);
};

