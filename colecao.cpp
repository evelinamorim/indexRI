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
int testeuou = 0;
int testepalavras = 0;
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
#include <cstring>
#include <ctime>
#include <htmlcxx/html/Node.h>
#include <htmlcxx/html/ParserDom.h>

using namespace std;
using namespace RICPNS;

const string Colecao::nome_arquivo_indice="index.bin";
const string Colecao::nome_arquivo_vocabulario="voc.txt";

Colecao::~Colecao(){

    delete escrita;
}

Colecao::Colecao(bool compacta){

    if (compacta) escrita = new EscreveCompacta(nome_arquivo_indice);
    else escrita = new EscreveNormal(nome_arquivo_indice);

    unordered_map<string,vector<int> >::iterator it_voc_inicio;
    unordered_map<string,vector<int> >::iterator it_voc_fim;

    buffer_chaves = new char*[TAM_VOC];
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
    clock_t  t;
    t = clock();
    while(leitor->getNextDocument(doc)){

	
	//cout << "[" << doc.getURL() << "]" << endl;
	tree<htmlcxx::HTML::Node> dom = parser.parseTree(doc.getText());
        ler_arvore_dom(dom,i,termos_pos);

	armazena_termos_doc(termos_pos,i);
	    
	//cout << dom << endl;

	++i;
    }
    t = clock() - t;
    cout << "Tempo Colecao::ler: "<< ((float)t/CLOCKS_PER_SEC) << "s" << endl;

    cout << "Colecao::Numero triplas: " << testeuou << endl;
    cout << "Colecao::Numero de palavras: " << testepalavras << endl;


    //finaliza o armazenamento aqui
    if(dynamic_cast<EscreveCompacta*>(escrita) != 0){
        if (escrita->pega_excedente()!=0)
	    escrita->escreve_excedente();
    }

    delete leitor;
}

 void Colecao::ler_arvore_dom(tree<htmlcxx::HTML::Node> dom,int idArvore,unordered_map<int,vector<int> >& termos_pos){
    // cout << "ler_arvore_dom " << idArvore << endl;
    tree<htmlcxx::HTML::Node>::iterator it = dom.begin();
    tree<htmlcxx::HTML::Node>::iterator end = dom.end();
    vector<string> listaPalavras;
    char* palavra = new char[MAIOR_PALAVRA+2];

    //para guardar relacao (termo,lista_posicoes_doc)

    vector<int> lpos;
    int i = 0;
    //contador para saber em que posicao esta uma dada palavra
    int palavraPos = 0; 

   // cout << "DOCUMENTO " << idArvore << " " << dom.size() << endl;


    for (; it != end; ++it,i++)
    {  
	    //aparentemente existe um header em toda pagina html
	    //entao eu ignoro este header aqui
	   if (i == 1) continue;

	    bool isscript;
	    string tag(it->tagName());

	    converteParaMinusculo(tag);

	    if (it->isTag()==1){
		if (tag=="script") isscript = true;
	        else isscript = false;
	    }


	     if (it->isComment()==0 && it->isTag()==0 && isscript==0){

		 listaPalavras.clear();

		 tokenizar(it->text(),listaPalavras);
		 int ii = 0;
	         vector<string>::iterator it_palavras = listaPalavras.begin();

	          while(ii<listaPalavras.size()){
	             // cout << "Percorrendo listaPalavras " << ii << endl;

		      memset(palavra,0,MAIOR_PALAVRA+2);
		      copy(listaPalavras[ii].begin(),listaPalavras[ii].end(),palavra);

		      palavraPos++;

	              int tamanho_palavra = strlen(palavra);

		      if (palavra!=NULL && tamanho_palavra!=0){
		          char* tmp = strdup(palavra);

		          converteParaMinusculo_char(tmp);
			  strcpy(palavra,tmp);
			  //se der falha eh aqui!
			  if (tmp!=NULL)
			     free(tmp);

			  testepalavras++;
		          if (vocabulario.find(palavra) == vocabulario.end()){

			       buffer_chaves[contaPalavras] = new char[tamanho_palavra+1];
		               memset(buffer_chaves[contaPalavras],0,tamanho_palavra+1);
			       strncpy(buffer_chaves[contaPalavras],palavra,tamanho_palavra);

			     // cout << "Armazenando " << contaPalavras << " " << buffer_chaves[contaPalavras] << endl;

	                      vocabulario[buffer_chaves[contaPalavras]] = contaPalavras;

		              vocabulario_invertido[contaPalavras] = buffer_chaves[contaPalavras];
 
			      termos_pos[contaPalavras] = vector<int>();
		              termos_pos[contaPalavras].push_back(palavraPos);

		              contaPalavras++;
		         }else{

			      if (termos_pos.find(vocabulario[palavra]) == termos_pos.end())
			           termos_pos[vocabulario[palavra]] = vector<int>();
		              termos_pos[vocabulario[palavra]].push_back(palavraPos);
		         }
		      } 
		     ii++;
		     it_palavras++;
	        }
	       //cout << "Fim da leitura de listaPalavras" << endl;	  
	    }  
	
    }
    if (palavra!=NULL)
       delete[] palavra;

    //cout << "Terminou ler_arvore_dom" << endl;

}

void Colecao::armazena_termos_doc(unordered_map<int,vector<int> >&  termos_pos,int doc){
    //dado o codigo do item lexical armazena em disco o 
    //lexical
   // cout << "armazena_termos_doc" << endl;

    unordered_map<int,vector<int> >::iterator it_termo = termos_pos.begin();
    vector<unsigned int> v;
    int pos_gap;


    //armazenando todos os docs!!os anteriores tbm!
    while(it_termo != termos_pos.end()){

	vector<int>::iterator it_pos;

	for(it_pos=it_termo->second.begin();it_pos!=it_termo->second.end();it_pos++){
	   if (it_pos!=it_termo->second.begin())
		pos_gap = *(it_pos)-*(it_pos-1);
	    else pos_gap = *it_pos;


            v.push_back(it_termo->first);
	    v.push_back(doc);
            v.push_back(*it_pos);
	    //cout << "==> " << *it_pos << " " << doc << " "  << it_termo->first << endl;

       
	   //  vector<unsigned int>().swap(v);
	}
       ++it_termo;
    }

    //TODO: estava dentro do laco de repeticao
    escrita->escreve_tripla(v);
    testeuou += v.size();
    //cout<<"Escrevendo "<<testeuou<<" triplas com "<<escrita->pega_conta_bits_global()<<endl;

    //cout << "Armazena: " << v.size() << endl;

    //TODO: acho que nao tem necessidade, mas...
    v.clear();

    it_termo = termos_pos.begin();

    while(it_termo != termos_pos.end()){
	it_termo->second.clear();
	it_termo++;
    }
    termos_pos.clear();
}

    
void Colecao::atualiza_vocabulario(int lex,unsigned long int pos){
    vocabulario[vocabulario_invertido[lex]] = pos;
}

void Colecao::escreve_vocabulario(){
    ofstream arquivo (nome_arquivo_vocabulario, ios::out|ios::app);
    if (arquivo.is_open()){

        unordered_map<char*,unsigned long int>::iterator it_voc = vocabulario.begin();
        int i=1;
	int ntermos = vocabulario_invertido.size();
        while(i<=ntermos){
	  arquivo << vocabulario_invertido[i] << " " <<  vocabulario[vocabulario_invertido[i]] << endl;
	    i++;
	}
	arquivo.close();
    }else{
	cout << "Colecao::atualiza_vocabulario::Problema ao abrir arquivo." << endl;
    }
    int n = vocabulario.size();
    if (buffer_chaves !=NULL){
        for(int i=0;i<n;i++){
	   if (buffer_chaves[i] !=NULL) 
	       delete[] buffer_chaves[i];
	}
        delete[] buffer_chaves;
    }
}

vector<int> Colecao::carrega_vocabulario(const string arquivo_vocabulario){
    ifstream arquivo(arquivo_vocabulario,ios::in);
    string linha;
    int i = 1;
    vector<int> posicoes;
    char* palavra = new char[MAIOR_PALAVRA+2]();
    int tamanho_palavra;

    if (arquivo.is_open()){
	while(getline(arquivo,linha)){
	    stringstream linhastream(linha);
	    string dado;
	    string lex;
	    int pos;

	    linhastream >> lex >> pos;
	    memset(palavra,0,MAIOR_PALAVRA+2);
            copy(lex.begin(),lex.end(),palavra);

	    if (palavra!= NULL){
		tamanho_palavra = strlen(palavra)+1;
	        buffer_chaves[i] = new char[tamanho_palavra+1];

		memset(buffer_chaves[i],0,tamanho_palavra+1);
                strncpy(buffer_chaves[i],palavra,tamanho_palavra);

	        vocabulario[buffer_chaves[i]] = i;
	        i++;
	        posicoes.push_back(pos);

	        vocabulario_invertido[i] = buffer_chaves[i];
	    }
	}
    }else{
	cout << "Colecao::carrega_vocabulario: Nao foi possivel abrir vocabulario." << endl;
    }
    if (palavra!=NULL) delete[] palavra;

   return posicoes; 

}

int Colecao::pega_lexico_inteiro(string p){
    char* palavra = new char[p.size()+2];
    memset(palavra,0,p.size()+1);
    int lex;
    copy(p.begin(),p.end(),palavra);
    if (palavra!=NULL){
       lex = vocabulario[palavra];
       delete[] palavra;
    }
    return lex;
}

char* Colecao::pega_lexico(int p){
    return vocabulario_invertido[p];
}

