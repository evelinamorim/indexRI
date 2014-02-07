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

#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;
using namespace RICPNS;



void Colecao::ler(string dirEntrada,string nomeIndice){
    CollectionReader* leitor = new CollectionReader(dirEntrada,nomeIndice);
    
    Document doc;

    doc.clear();

    int i = 0;
    while(leitor->getNextDocument(doc)){

	//if((i%1000) == 0){
	
	    cout << "[" << doc.getText() << "]" << endl;
	    //ja tem o HTML e agora eh usar o parser de html
	//}
	++i;
    }

    delete leitor;
}
