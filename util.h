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
#include <vector>
#include <sys/stat.h>
#include <cmath>

#ifndef __UTIL_H_
#define __UTIL_H_

using namespace std;

void converteParaMinusculo(string& s);

vector<string> tokenizar(string s);

bool ehPontuacao(char c);

bool arquivoExiste(const string nomeArquivo);

int para_codigo_unario(int x);

void para_codigo_gamma(unsigned int x,unsigned int& y,unsigned int &ny);

unsigned int unario_para_int(vector<unsigned int>& x);

unsigned int gamma_para_int(vector<unsigned int>& x,int& nx,int pos);

#endif
