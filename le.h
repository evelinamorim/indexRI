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
#include <fstream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

class Le{
    vector<unsigned int> buffer;
    string nome_arquivo;
    int conta_bits;
    public:
    Le(string narquivo);
    int ler_tripla(unsigned int& lex,unsigned int& doc,unsigned int& pos);
    int ler_numero();
    void carrega_buffer(ifstream& arquivo);
    int pega_conta_bits();
};
