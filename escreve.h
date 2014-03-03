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

#ifndef __ESCREVE_H_
#define __ESCREVE_H_

class Escreve{
    protected:
    unsigned int conta_bits;
    unsigned int conta_bits_global;
    unsigned int* buffer;
    vector<int> pilha_buffer;
    string nome_arquivo;

    public:
    Escreve(string narquivo);

    //metodos get's e set's
    unsigned int pega_conta_bits_global();
    unsigned int pega_conta_bits();
    string pega_nome_arquivo();
    void inicia_conta_bits(unsigned int cb);

    //metodos de escrita
    int escreve_tripla(vector<unsigned int> v);

    //metodos virtuais puros
    virtual void escreve_numero(unsigned int x) =0;
    virtual void carrega_buffer(int tam_buffer) =0;
    virtual void escreve_buffer(ofstream& arquivo) =0;

    //metodos virtuais nao puros
    virtual void inicia_excedente(unsigned int e){};
    virtual void escreve_excedente(){};
    virtual unsigned int pega_excedente(){return 0;};
};

class EscreveNormal: public Escreve{
    public:
    EscreveNormal(string narquivo);
    //metodos de escrita
    void escreve_numero(unsigned int x);
    void carrega_buffer(int tam_buffer);
    void escreve_buffer(ofstream& arquivo);
};


class EscreveCompacta: public Escreve{
    //guarda o excedente do buffer
    unsigned int *excedente;
    public:
    EscreveCompacta(string narquivo);

    //metodos de escrita
    void escreve_numero(unsigned int x);
    void carrega_buffer(int tam_buffer);
    void escreve_buffer(ofstream& arquivo);

    //metodos relacionados com o excedente de bits que 
    //a escrita compactada pode gerar
    void inicia_excedente(unsigned int e);
    void escreve_excedente();
    unsigned int pega_excedente();
};

#endif
