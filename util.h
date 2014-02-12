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

using namespace std;

void converteParaMinusculo(string& s);

vector<string> tokenizar(string s);

bool ehPontuacao(char c);

