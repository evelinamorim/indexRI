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
#include <cstdio>
#include <regex>

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

	
	cout << "[" << doc.getURL() << "]" << endl;
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

    int i = 0;
    for (; it != end; ++it,i++)
    {  
	    //aparentemente existe um header em toda pagina html
	    //entao eu ignoro este header aqui
	    if (i == 1) continue;

	    bool isscript;
	    string tag = it->tagName();

	    converteParaMinusculo(tag);

	    if (it->isTag()==1){
		if (tag=="script") isscript = true;
	        else isscript = false;
	    }

	    //cout << "(1) Tag: " << tag << endl; 

	    if (it->isComment()==0 && it->isTag()==0 && isscript==0){

		vector<string> listaPalavras = tokenizar(it->text());
		vector<string>::iterator it = listaPalavras.begin();


	       /*cout << "------------------------------------------" << endl;
	        cout << "(2) Tag: " <<  tag << endl;
	        cout << it->text() << endl;

	        cout << "------------------------------------------" << endl;*/


	        while(it!=listaPalavras.end()){
		   converteParaMinusculo(*it);

		   int& valor = vocabulario[*it];

		   //TODO: pegar offset do no + offset da palavra
		   if (valor == 0){
		       cout << "Palavra: " << *it << " Codigo: " << contaPalavras << endl;
	               vocabulario[*it] = contaPalavras;
		       contaPalavras++;
		   }
		   it++;
	        }
	    }
	
    }
}
