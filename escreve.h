/*
 * =====================================================================================
 *
 *       Filename:  escreve.h
 *
 *    Description:  Header da classe que escreve em disco lista invertida
 *
 *        Version:  1.0
 *        Created:  02/17/14 18:48:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Escreve{
    unsigned int* buffer;
    int conta_bits;
    string nome_arquivo;
    public:
    Escreve(string narquivo);
    void escreve_lista(int lex,int doc,vector<unsigned int> v);
    void escreve_numero(unsigned int x);
    void escreve_buffer(ofstream& arquivo);
};

