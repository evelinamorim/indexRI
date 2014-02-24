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
    vector<int> pilha_buffer;
    //guarda o excedente do buffer
    unsigned int *excedente;
    int conta_bits;
    string nome_arquivo;
    public:
    Escreve(string narquivo);
    int pega_conta_bits();
    string pega_nome_arquivo();
    int escreve_tripla(vector<unsigned int> v);
    void escreve_numero(unsigned int x);
    void carrega_buffer(int tam_buffer);
    void escreve_buffer(ofstream& arquivo);
    void inicia_conta_bits(int cb);
    void inicia_excedente(unsigned int e);
    void escreve_excedente();
    unsigned int pega_excedente();
};

