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
	tree<htmlcxx::HTML::Node> dom = parser.parseTree(doc.getText(),i);

	lerArvoreDom(dom);
	//cout << dom << endl;

	++i;
    }

    delete leitor;
}

void Colecao::lerArvoreDom(tree<htmlcxx::HTML::Node> dom,int idArvore){
    tree<htmlcxx::HTML::Node>::iterator it = dom.begin();
    tree<htmlcxx::HTML::Node>::iterator end = dom.end();

    //para guardar relacao (termo,lista_posicoes_doc)
    unordered_map<int,vector<int>> termos_pos;

    int i = 0;
    //contador para saber em que posicao esta uma dada palavra
    int palavraPos = 0; 

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


	    if (it->isComment()==0 && it->isTag()==0 && isscript==0){

		vector<string> listaPalavras = tokenizar(it->text());
		vector<string>::iterator it = listaPalavras.begin();




	        while(it!=listaPalavras.end()){
		   converteParaMinusculo(*it);

		   palavraPos++;

		   int& valor = vocabulario[*it];

		   if (valor == 0){
	               vocabulario[*it] = contaPalavras;
		       termos_pos[contaPalavras].push_back(palavraPos);
		       contaPalavras++;
		   }else{
		       //TODO: esta correto esta representacao para gap?
		       int gap = termo_pos[vocabulario[*it].size()-1]-palavraPos;
		       termos_pos[vocabulario[*it]].push_back(gap);
		   }
		   it++;
	        }
	    }
	
    }
}

void Colecao::armazenaTermosDoc(unordered_map<int,vector<int>> termos_pos,int doc){
    //dado o codigo do item lexical armazena em disco o 
    //lexical

    unordered_map<int,vector<int>>::iterator it_termo = termos_pos.begin();

    while(it_termo != termos_pos.end()){
       armazenaTermo(it_termo->first,it_termo->second,doc);
       ++it_termo;
    }
}

void Colecao::armazenaTermo(int lex,int doc,vector<int> posicoes){

    //tem que verificar se o arquivo existe para nao sobrescrever
    ofstream arquivo(nomeArquivoIndice,ios::out|ios::binary);
    if (arquivo.good()){
	arquivo.write();
    }
}
