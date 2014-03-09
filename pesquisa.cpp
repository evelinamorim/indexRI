/*
 * =====================================================================================
 *
 *       Filename:  pesquisa.cpp
 *
 *    Description:  Arquivo que realiza a pesquisa em um arquivo de indice 
 *    invertido
 *
 *        Version:  1.0
 *        Created:  02/25/14 22:50:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <unordered_map>
#include <cstring>
#include <sstream>

#include "pesquisa.h"
#include "CollectionReader.h"
#include "util.h"

#define INTERSECAO 1
#define UNIAO 2

using namespace std;
using namespace RICPNS;

const string Pesquisa::nome_arquivo_vocabulario = "voc.txt";
const string Pesquisa::nome_arquivo_indice = "index.bin";

Pesquisa::Pesquisa(bool compacta){
    col = new Colecao(compacta);

    if (compacta){
	leitura = new LeCompacta(nome_arquivo_indice);
    }else{
	leitura = new LeNormal(nome_arquivo_indice);
    }

    posicoes_palavras = col->carrega_vocabulario(nome_arquivo_vocabulario);
    
}

vector<string> Pesquisa::processa_consulta(string consulta,int& tipo_consulta){
    //vetor vazio se houver problema com a consulta
    stringstream ss(consulta);
    vector<string> lista_consulta;
    string palavra;

    while(ss>>palavra){
	converteParaMinusculo(palavra);
	if (palavra == "and"){
	    tipo_consulta = INTERSECAO;
	}else{
	    if (palavra == "or") tipo_consulta = UNIAO;
	    else lista_consulta.push_back(palavra);
	}
    }
    return lista_consulta;
}

void Pesquisa::intersecao(unordered_map<unsigned int,vector<unsigned int> >& r1, unordered_map<unsigned int,vector<unsigned int> >& r2){

    unordered_map<unsigned int,	vector<unsigned int> >::iterator it2;

    it2=r2.begin();

    vector<unsigned int> elementos_diferentes;

    while(it2!=r2.end()){
	if (r1.find(it2->first) == r1.end()){
	    elementos_diferentes.push_back(it2->first);
	}
	it2++;
    }

    vector<unsigned int>::iterator it_dif;
    for(it_dif=elementos_diferentes.begin();it_dif!=elementos_diferentes.end();it_dif++){
	r2.erase(*it_dif);
    }

}


unordered_map<unsigned int,vector<unsigned int> > Pesquisa::executa(string palavra){
    int i = col->pega_lexico_inteiro(palavra);
    int pos_arquivo,freq,final_arquivo;
    unsigned int doc;
    unordered_map<unsigned int,vector<unsigned int> > resultado;

    if (i == posicoes_palavras.size()){
	ifstream arquivo(nome_arquivo_indice,ios::in|ios::binary|ios::ate);
	//TODO: e se ler os bits finais que estao zerados?
	pos_arquivo = arquivo.tellg()*8;
	arquivo.close();
    }else  pos_arquivo = posicoes_palavras[i];

    vector<unsigned int> v;

    leitura->inicia_conta_bits(posicoes_palavras[i-1]);

    final_arquivo = 1;

    while (leitura->pega_conta_bits()<pos_arquivo && final_arquivo != -1){
          leitura->ler_tripla(v,2);
	  freq = v.back();
	  v.pop_back();
	  doc = v.back();
	  v.pop_back();

	  v.clear();
          final_arquivo = leitura->ler_tripla(v,freq);
          resultado[doc] = v;

	  v.clear();
    }


    return resultado;

}

void Pesquisa::imprime_docs_resultados(unordered_map<unsigned int,vector<unsigned int> > resultado,string dir_entrada,string nome_indice)
{
    //dado o resultado de uma consulta. Percorre os documentos da base 
    //para imprimir aqueles do resultado
    CollectionReader* leitor = new CollectionReader(dir_entrada,nome_indice);
    Document doc;

    unsigned int i = 1;
    while(leitor->getNextDocument(doc)){

	if (resultado.find(i)!=resultado.end()){
	   cout << "DOCUMENTO " << i << endl;
	   cout << "[" << doc.getURL() << "]" << endl;
	  // cout << doc.getText() << endl << endl;
	}
	///tree<htmlcxx::HTML::Node> dom = parser.parseTree(doc.getText());


	++i;
    }
}
 

int main(int argc,char** argv){
    Pesquisa* p;
    bool compacta = false;
    string dir_entrada = argv[1];
    string nome_indice = argv[2];

    if (argc == 4){
	if (strncmp(argv[3],"-c",2)==0) compacta = true;
    }

   p = new Pesquisa(compacta);
   vector<string> consulta;
   int tipo_consulta;
   string palavra;
   unordered_map<unsigned int,vector<unsigned int> > resultado,resultado1,resultado2;

    while (getline(cin,palavra)){
	cout << "Pesquisa da palavras: " << palavra << endl;
	consulta = p->processa_consulta(palavra,tipo_consulta);

	if (consulta.size()>1){
            resultado1 = p->executa(consulta[0]);
            resultado2 = p->executa(consulta[1]);
	    if (tipo_consulta == INTERSECAO){
	       p->intersecao(resultado1,resultado2);
               p->imprime_docs_resultados(resultado2,dir_entrada,nome_indice);
	    }else{
               p->imprime_docs_resultados(resultado1,dir_entrada,nome_indice);
               p->imprime_docs_resultados(resultado2,dir_entrada,nome_indice);
	   }
	}else{
            resultado = p->executa(consulta[0]);
            p->imprime_docs_resultados(resultado,dir_entrada,nome_indice);
	}
	cout << endl;
    }
    return 0;
}

