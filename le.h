/*
 * =====================================================================================
 *
 *       Filename:  le.h
 *
 *    Description:  arquivo header para leitura de arquivos de indice invertido
 *
 *        Version:  1.0
 *        Created:  02/18/14 23:08:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <iostream>
#include <deque>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

class Le{
    protected:
    deque<unsigned int> buffer;
    string nome_arquivo;
    unsigned long int conta_bits;
    streampos tamanho_arquivo;

    public:
    Le(string narquivo);
    virtual ~Le(){};
    //metodos de get's e set's
    void inicia_nome_arquivo(string narquivo);
    void inicia_conta_bits(unsigned int cb);
    unsigned long int pega_conta_bits();

    int ler_tripla(deque<unsigned int>& v,int nnum);
    int ler_tripla_pos(deque<unsigned int>& v,int pos);

    //metodos virtuais
    virtual int ler_numero() =0;
    virtual void carrega_buffer(ifstream& arquivo,int nnum) =0;
};


class LeNormal:public Le{
    public:
    LeNormal(string narquivo);
    ~LeNormal();
    int ler_numero();
    void carrega_buffer(ifstream& arquivo,int nnum);
};

class LeCompacta: public Le{
    public:
    LeCompacta(string narquivo);
    ~LeCompacta();
    int ler_numero();
    void carrega_buffer(ifstream& arquivo,int nnum);
};
