/*
 * =====================================================================================
 *
 *       Filename:  documento.cpp
 *
 *    Description:  Arquivo que manipula documentos do repositorio
 *
 *        Version:  1.0
 *        Created:  02/06/14 22:37:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  evelinamorim, 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "colecao.h"
#include "CollectionReader.h"
#include "util.h"

#include <cstdlib>
#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>

using namespace std;
using namespace RICPNS;

//carregar todas as palavras do vocabulario em memoria, para poder contar
//a quantidade de palavras e inserir a localizacao? Esta localizacao 
//eh a posicao no HTML em bytes? como?
//
//Posso usar o map para montar meu vocabulario?

Colecao::Colecao(){
    contaPalavras = 1;
}

void Colecao::ler(string dirEntrada,string nomeIndice){
    CollectionReader* leitor = new CollectionReader(dirEntrada,nomeIndice);
    Document doc;

    htmlcxx::HTML::ParserDom parser;

    doc.clear();

    int i = 0;
    while(leitor->getNextDocument(doc)){

	
	//cout << "[" << doc.getText() << "]" << endl;
	tree<htmlcxx::HTML::Node> dom = parser.parseTree(doc.getText());

	lerArvoreDom(dom);
	//cout << dom << endl;

	++i;
    }

    delete leitor;
}

void Colecao::lerArvoreDom(tree<htmlcxx::HTML::Node> dom){
    tree<htmlcxx::HTML::Node>::iterator it = dom.begin();
    tree<htmlcxx::HTML::Node>::iterator end = dom.end();
    for (; it != end; ++it)
    {  
	if (it->text().length()>0){
	    istringstream iss(it->text());
	    string palavra;

	    while(iss >> palavra){
		converteParaMinusculo(palavra);

		int& valor = vocabulario[palavra];

		//TODO: porque ele esta pegando algumas coisas de html?
		if (valor == 0){
		    cout << "Palavra: " << palavra << " Codigo: " << contaPalavras << endl;
	           vocabulario[palavra] = contaPalavras;
		   contaPalavras++;
		}
	    }
	}
    }
}
