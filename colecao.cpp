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
#include "ordena.h"

#include <cstdlib>
#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <cstdio>
#include <regex>

using namespace std;
using namespace RICPNS;

const string Colecao::nome_arquivo_indice="index.bin";
const string Colecao::nome_arquivo_vocabulario="voc.txt";

Colecao::Colecao():escrita(nome_arquivo_indice){
    contaPalavras = 1;
}

const string Colecao::pega_nome_arquivo_indice(){
    return nome_arquivo_indice;
}

int Colecao::pega_tamanho_vocabulario(){
    return vocabulario.size();
}

void Colecao::ler(string dirEntrada,string nomeIndice){
    CollectionReader* leitor = new CollectionReader(dirEntrada,nomeIndice);
    Document doc;

    unordered_map<int,vector<int> > termos_pos;

    htmlcxx::HTML::ParserDom parser;

    doc.clear();

    int i = 1;
    while(leitor->getNextDocument(doc)){

	
	cout << "[" << doc.getURL() << "]" << endl;
	tree<htmlcxx::HTML::Node> dom = parser.parseTree(doc.getText());

	termos_pos = ler_arvore_dom(dom,i);
	armazena_termos_doc(termos_pos,i);
	    
	//cout << dom << endl;

	++i;
    }

    //finaliza o armazenamento aqui
    if (escrita.pega_excedente()!=0)
	escrita.escreve_excedente();


    delete leitor;
}

unordered_map<int,vector<int> > Colecao::ler_arvore_dom(tree<htmlcxx::HTML::Node> dom,int idArvore){
    tree<htmlcxx::HTML::Node>::iterator it = dom.begin();
    tree<htmlcxx::HTML::Node>::iterator end = dom.end();

    //para guardar relacao (termo,lista_posicoes_doc)
    unordered_map<int,vector<int> > termos_pos;

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
		       vocabulario_invertido[contaPalavras] = *it;

		       termos_pos[contaPalavras].push_back(palavraPos);
		       contaPalavras++;
		   }else{
		       //esta logica de gap soh funciona para o primeiro da lista
		       //int gap = termos_pos[vocabulario[*it]].back()-palavraPos;
		       termos_pos[vocabulario[*it]].push_back(palavraPos);
		   }
		   it++;
	        }
	    }
	
    }

    return termos_pos;
}

void Colecao::armazena_termos_doc(unordered_map<int,vector<int> > termos_pos,int doc){
    //dado o codigo do item lexical armazena em disco o 
    //lexical

    unordered_map<int,vector<int> >::iterator it_termo = termos_pos.begin();
    vector<unsigned int> v;
    int pos_gap;

    while(it_termo != termos_pos.end()){

	vector<int>::iterator it_pos;

	for(it_pos=it_termo->second.begin();it_pos!=it_termo->second.end();it_pos++)        {
	    if (it_pos!=it_termo->second.begin())
		pos_gap = *(it_pos)-*(it_pos-1);
	    else pos_gap = *it_pos;


            v.push_back(it_termo->first);
	    v.push_back(doc);
            v.push_back(pos_gap);

	    //cout << "-->" << it_termo->first << " " << doc << " " << pos_gap << endl;

	    escrita.escreve_tripla(v);

            v.clear();
	}
	
       ++it_termo;
    }
}

    
void Colecao::atualiza_vocabulario(int lex,int pos){
    //if (vocabulario_invertido[lex] == "tecnologia")
    //    cout <<">>>> " << vocabulario_invertido[lex] << " " << vocabulario[vocabulario_invertido[lex]] << " " << lex << " " << pos << endl;
    vocabulario[vocabulario_invertido[lex]] = pos;
}

void Colecao::escreve_vocabulario(){
    ofstream arquivo (nome_arquivo_vocabulario, ios::out|ios::app);
    if (arquivo.is_open()){

        unordered_map<string,int>::iterator it_voc = vocabulario.begin();
        int i=1;
	int ntermos = vocabulario_invertido.size();
        while(i<ntermos){
	    arquivo << vocabulario_invertido[i] << " " <<  vocabulario[vocabulario_invertido[i]] << endl;
	    i++;
	}
	arquivo.close();
    }else{
	cout << "Colecao::atualiza_vocabulario::Problema ao abrir arquivo." << endl;
    }
}

vector<int> Colecao::carrega_vocabulario(const string arquivo_vocabulario){
    ifstream arquivo(arquivo_vocabulario,ios::in);
    string linha;
    int i = 1;
    vector<int> posicoes;

    if (arquivo.is_open()){
	while(getline(arquivo,linha)){
	    stringstream linhastream(linha);
	    string dado;
	    string lex;
	    int pos;

	    linhastream >> lex >> pos;

	    vocabulario[lex] = i;
	    i++;
	    posicoes.push_back(pos);

	    //pos pode ser um hash, pois nao faz sentido ter uma palavra com posicao
	    //igual
	    vocabulario_invertido[i] = lex;
	}
    }else{
	cout << "Colecao::carrega_vocabulario: Nao foi possivel abrir vocabulario." << endl;
    }

   return posicoes; 

}

int Colecao::pega_lexico_inteiro(string p){
    return vocabulario[p];
}

