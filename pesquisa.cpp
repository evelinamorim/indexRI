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

#include "pesquisa.h"
#include "CollectionReader.h"

using namespace std;

const string Pesquisa::nome_arquivo_vocabulario = "voc.txt";
const string Pesquisa::nome_arquivo_indice = "index.bin";

Pesquisa::Pesquisa():col(nome_arquivo_indice){
    posicoes_palavras = col.carrega_vocabulario(nome_arquivo_vocabulario);
    
}

unordered_map<int,vector<int> > Pesquisa::executa(string palavra){
    int i = col.pega_lexico_inteiro(palavra);
    int pos_arquivo;
    unordered_map<int,vector<int> > resultado;

    if (i == posicoes_palavras.size()-1){
	ifstream arquivo(nome_arquivo_indice,ios::in|ios::binary|ios:ate);
	//TODO: e se ler os bits finais que estao zerados?
	pos_arquivo = arquivo.tellg()*8;
	arquivo.close();
    }else  pos_arquivo = posicoes_palavras[i+1];

    vector<int> v;

    leitura.inicia_conta_bits(posicoes_palavras[i]);
    leitura.ler_tripla_pos(v,pos_arquivo);

    vector<int>::iterator it;
    for(it=v.begin();it!=v.end();it++){
	int pos = v.back();
	v.push_back();
	int doc = v.back();
	v.push_back();
        resultado[doc].push_back(pos);
    }

    return resultado;

}

void imprime_docs_resultados(<unordered_map<int,vector<int> > resultado){
    //dado o resultado de uma consulta. Percorre os documentos da base 
    //para imprimir aqueles do resultado
    CollectionReader* leitor = new CollectionReader(dirEntrada,nomeIndice);
    Document doc;

    int i = 1;
    while(leitor->getNextDocument(doc)){

	int& valor = resultado[i];
	if (valor !=0){
	   cout << "[" << doc.getURL() << "]" << endl;
	   cout << doc.getText() << endl << endl;
	}
	///tree<htmlcxx::HTML::Node> dom = parser.parseTree(doc.getText());


	++i;
    }
}

int main(){
    return 0;
}

