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


#include <vector>

using namespace std;

class Colecao{
    //lista de documentos
    //TODO: nao carregar tudo na memoria?
    //vector<Documento> lista;
    public:
    void ler(string dirEntrada,string nomeIndice);
};

