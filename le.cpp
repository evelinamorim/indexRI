/*
 * =====================================================================================
 *
 *       Filename:  le.cpp
 *
 *    Description:  Le dados de um arquivo de indice invertido
 *
 *        Version:  1.0
 *        Created:  02/18/14 23:06:21
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "le.h"
#include "util.h"

using namespace std;

Le::Le(string narquivo){
    conta_bits = 0;
    nome_arquivo = narquivo; 
}

int Le::pega_conta_bits(){
    return conta_bits;
}

int Le::ler_tripla(unsigned int& lex,unsigned int& doc,unsigned int& pos){
    //esta funcao le apenas uma tripla, dada a posicao atual do arquivo
    //depois atualiza pos com a nova posicao
    ifstream arquivo(nome_arquivo,ios::in|ios::binary);
    int pos_arquivo;

    if (arquivo.is_open()){

        pos_arquivo = floor(conta_bits/32);
	arquivo.seekg(sizeof(int)*pos_arquivo,ios::beg);
	carrega_buffer(arquivo);

	lex = ler_numero();
	doc = ler_numero();
	pos = ler_numero();
	pos_arquivo = arquivo.tellg();

	arquivo.close();
    }else{
	cout << "Ler::ler_tripla::Unable to open file." << endl;
    }

    return pos_arquivo;

}

void Le::carrega_buffer(ifstream& arquivo){
    //carrega buffer com os numeros a serem decodificados

    //tres inteiros porque estou assumindo que sempre lerei ao 
    //menos tres inteiros (tripla)
    buffer.clear();

    unsigned int *buf = new unsigned int[3];

    //cout << ">>> " << arquivo.tellg() << endl;
    //TODO: tah lendo um inteiro so!
    arquivo.read((char*) buf,sizeof(int));
    //cout << "6: " << arquivo.tellg() << endl;
    arquivo.read((char*) (buf+1),sizeof(int));
    //cout << "6: " << arquivo.tellg() << endl;
    arquivo.read((char*) (buf+2),sizeof(int));
    //cout << "4: " << buf[1] <<endl;
    //cout << "5: " << buf[2] <<endl << endl;

    //bits ja lidos devem ser zerados. Devem existir pos_bit's para zerar
    int pos_bit = conta_bits % 32;

    for(int i=31;i>(31-pos_bit);i--){
	 buf[0] &= ~(1 << i);
    }


    buffer.push_back(buf[0]);
    buffer.push_back(buf[1]);
    buffer.push_back(buf[2]);
}

int Le::ler_numero(){

    //TODO: repensar esta conta, porque so leio de tres em tres inteiros
    //int pos_buffer = ceil(conta_bits/32);
    int pos_bit = conta_bits % 32;
    int nx;

    int numero = gamma_para_int(buffer,nx,31-pos_bit);

    conta_bits += nx;

    return numero;
}

int main(){
    Le l = Le("teste_comp5.bin");
    unsigned int lex,doc,pos;

    l.ler_tripla(lex,doc,pos);
    cout << lex << " " << doc << " " << pos << endl;
    l.ler_tripla(lex,doc,pos);
    cout << lex << " " << doc << " " << pos << endl;
    l.ler_tripla(lex,doc,pos);
    cout << lex << " " << doc << " " << pos << endl;
    l.ler_tripla(lex,doc,pos);

    cout << lex << " " << doc << " " << pos << endl;
    l.ler_tripla(lex,doc,pos);
    cout << lex << " " << doc << " " << pos << endl;
    l.ler_tripla(lex,doc,pos);
    cout << lex << " " << doc << " " << pos << endl;
    l.ler_tripla(lex,doc,pos);
    cout << lex << " " << doc << " " << pos << endl;
    l.ler_tripla(lex,doc,pos);
    cout << lex << " " << doc << " " << pos << endl;

    l.ler_tripla(lex,doc,pos);
    cout << lex << " " << doc << " " << pos << endl;
    l.ler_tripla(lex,doc,pos);
    cout << lex << " " << doc << " " << pos << endl;
    l.ler_tripla(lex,doc,pos);
    cout << lex << " " << doc << " " << pos << endl;

    l.ler_tripla(lex,doc,pos);
    cout << lex << " " << doc << " " << pos << endl;
    l.ler_tripla(lex,doc,pos);
    cout << lex << " " << doc << " " << pos << endl;
    l.ler_tripla(lex,doc,pos);
    cout << lex << " " << doc << " " << pos << endl;

    l.ler_tripla(lex,doc,pos);
    cout << lex << " " << doc << " " << pos << endl;
    l.ler_tripla(lex,doc,pos);
    cout << lex << " " << doc << " " << pos << endl;
    l.ler_tripla(lex,doc,pos);
    cout << lex << " " << doc << " " << pos << endl;
    return 0;
}

