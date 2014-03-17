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
#include <cstring>
#include <fstream>
#include <sstream>

using namespace std;

#ifndef __COLECAO_H_
#define __COLECAO_H_




struct  eqstr {
    inline bool operator() (char const *s1,char const *s2) const {
	// cout << "==> " << s1 << " " << " "  << s2 << " " << strcmp(s1,s2) << endl;
	 if (s1==NULL){
	     if (s2==NULL){
		 return true;
	     }else return false;
	 }

	 if (s2==NULL){
	     return false;
	 }

	 int len1 = strlen(s1);
	 int len2 = strlen(s2);

	 if (len1 == 0){
	     if (len2 == 0){
		 return true;
	     }else return false;
         }
	 if (len2 == 0)  return false;
	 int min = len1 < len2 ? len1:len2; 

	 //problema aqui: Invalid read of size 1
	 return strcmp(s1,s2)==0;
    }
}; 

struct Hash_Func{
       int operator() (char* k) const{
	   string s("");
	   stringstream ss;


	   if (k != NULL){
	      s.reserve(strlen(k)+1);
	      ss << k; //problema aqui
	      ss >> s;// assuming k is a C-string
	   }
	   static hash<string> hash_function; // standard hashing functor
           int h = hash_function(s);
	   //cout << "==> " << s << " " << h << endl;
	   return h;
	}
     
        //BKDR hash algorithm
	/*int operator()(char * str)const
	{
	    int seed = 131;//31  131 1313 13131131313 etc//
	    int hash = 0;
	    while(*str){
	       hash = (hash * seed) + (*str);
	        str ++;
	    }
	    return hash & (0x7FFFFFFF);
	                                                                                   }*/ 
                                                                                    };

class Colecao{
    //lista de documentos
    //TODO: nao carregar tudo na memoria?
    //vector<Documento> lista;
    unordered_map<char*,unsigned long int,Hash_Func,eqstr> vocabulario;
    unordered_map<unsigned long int,char*> vocabulario_invertido;

    char** buffer_chaves;
    int contaPalavras;
    static const string nome_arquivo_vocabulario;
    static const string nome_arquivo_indice;
    Escreve *escrita;

    public:
    Colecao(bool compacta);
    ~Colecao();
    void ler(string dirEntrada,string nomeIndice);
    void ler_arvore_dom(tree<htmlcxx::HTML::Node> arvoreDom,int doc,unordered_map<int,vector<int> >& termos_pos);
    vector<string> lerLexicais(string sentenca);
    void armazena_termos_doc(unordered_map<int,vector<int> >& termos_pos,int doc);
    const string pega_nome_arquivo_indice();
    int pega_tamanho_vocabulario();

    void atualiza_vocabulario(int lex,unsigned long int pos);
    void escreve_vocabulario();
    vector<int>  carrega_vocabulario(const string arquivo_vocabulario);
    int pega_lexico_inteiro(string p);
    char*  pega_lexico(int p);

};

#endif
