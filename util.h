/*
 * =====================================================================================
 *
 *       Filename:  util.h
 *
 *    Description:  biblioteca de funcoes uteis
 *
 *        Version:  1.0
 *        Created:  02/10/14 14:20:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  evelin amorim, 
 *   Organization:  
 *
 * =====================================================================================
 */


#include <algorithm>
#include <string>
#include <deque>
#include <vector>
#include <sys/stat.h>
#include <cmath>

#ifndef __UTIL_H_
#define __UTIL_H_

#define MAIOR_PALAVRA 20
#define TAM_VOC 20000000
//TODO: depois mudar este tamanho. No momento eh soh para teste
#define TAMANHO_RUN 5000000

using namespace std;

void converteParaMinusculo_char(char* s);
void converteParaMinusculo(string& s);

void tokenizar(string s,vector<string>& v);

bool ehPontuacao(char c);

bool arquivoExiste(const string nomeArquivo);

int para_codigo_unario(unsigned int x);

void para_codigo_gamma(unsigned int x,unsigned int& y,unsigned int &ny);

unsigned int unario_para_int(deque<unsigned int>& x,int pos);

unsigned int gamma_para_int(deque<unsigned int>& x,unsigned int& nx,int pos);

#endif
