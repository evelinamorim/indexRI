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

    public:
    Colecao();
    void ler(string dirEntrada,string nomeIndice);
    void lerArvoreDom(tree<htmlcxx::HTML::Node> arvoreDom);
    vector<string> lerLexicais(string sentenca);
};

