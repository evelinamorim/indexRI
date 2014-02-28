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
#include "pesquisa.h"
#include "CollectionReader.h"

using namespace std;
using namespace RICPNS;

const string Pesquisa::nome_arquivo_vocabulario = "voc.txt";
const string Pesquisa::nome_arquivo_indice = "index.bin";

Pesquisa::Pesquisa():leitura(nome_arquivo_indice){
    posicoes_palavras = col.carrega_vocabulario(nome_arquivo_vocabulario);
    
}

unordered_map<unsigned int,vector<unsigned int> > Pesquisa::executa(string palavra){
    int i = col.pega_lexico_inteiro(palavra);
    int pos_arquivo;
    unordered_map<unsigned int,vector<unsigned int> > resultado;

    if (i == posicoes_palavras.size()-1){
	ifstream arquivo(nome_arquivo_indice,ios::in|ios::binary|ios::ate);
	//TODO: e se ler os bits finais que estao zerados?
	pos_arquivo = arquivo.tellg()*8;
	arquivo.close();
    }else  pos_arquivo = posicoes_palavras[i];

    vector<unsigned int> v;

    leitura.inicia_conta_bits(posicoes_palavras[i-1]);
    leitura.ler_tripla_pos(v,pos_arquivo);

    for(int i=0;i<v.size();i+=2){
	unsigned int doc = v[i];
	unsigned int pos = v[i+1];
        resultado[doc].push_back(pos);
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
	   cout << doc.getText() << endl << endl;
	}
	///tree<htmlcxx::HTML::Node> dom = parser.parseTree(doc.getText());


	++i;
    }
}
 

int main(int argc,char** argv){
    Pesquisa p;
    string dir_entrada = argv[1];
    string nome_indice = argv[2];
    unordered_map<unsigned int,vector<unsigned int> > resultado = p.executa("tecnologia");
    p.imprime_docs_resultados(resultado,dir_entrada,nome_indice);
    return 0;
}

