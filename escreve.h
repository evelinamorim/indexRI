
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
    unsigned long int conta_bits;
    unsigned long int conta_bits_global;
    unsigned int* buffer;
    vector<int> pilha_buffer;
    string nome_arquivo;

    public:
    Escreve(string narquivo);
    virtual ~Escreve(){};
    //metodos get's e set's
    unsigned long int pega_conta_bits_global();
    unsigned long int pega_conta_bits();
    string pega_nome_arquivo();
    void inicia_conta_bits(unsigned long int cb);

    //metodos de escrita
    streampos escreve_tripla(vector<unsigned int> v);

    //metodos virtuais puros
    virtual void escreve_numero(unsigned int x,int tam_buffer) =0;
    virtual void carrega_buffer(int tam_buffer) =0;
    virtual void escreve_buffer(ofstream& arquivo,int tam_buffer) =0;

    //metodos virtuais nao puros
    virtual void inicia_excedente(unsigned int e){};
    virtual void escreve_excedente(){};
    virtual unsigned int pega_excedente(){return 0;};
};

class EscreveNormal: public Escreve{
    public:
    EscreveNormal(string narquivo);
    ~EscreveNormal();
    //metodos de escrita
    void escreve_numero(unsigned int x,int tam_buffer);
    void carrega_buffer(int tam_buffer);
    void escreve_buffer(ofstream& arquivo,int tam_buffer);
};


class EscreveCompacta: public Escreve{
    //guarda o excedente do buffer
    unsigned int *excedente;
    public:
    EscreveCompacta(string narquivo);
    ~EscreveCompacta();

    //metodos de escrita
    void escreve_numero(unsigned int x,int tam_buffer);
    void carrega_buffer(int tam_buffer);
    void escreve_buffer(ofstream& arquivo,int tam_buffer);

    //metodos relacionados com o excedente de bits que 
    //a escrita compactada pode gerar
    void inicia_excedente(unsigned int e);
    void escreve_excedente();
    unsigned int pega_excedente();
};

#endif

