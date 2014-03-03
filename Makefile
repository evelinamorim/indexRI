ricode = ../code/src/
ridata = /Users/evelinamorim/Documents/Ufmg/2014-1/RI/
riindex = index.txt

ziplib : $(ricode) 
	g++ -c -o $(ricode)/CollectionReader.o $(ricode)/CollectionReader.cpp -g
	g++ -c -o $(ricode)/CollectionWriter.o $(ricode)/CollectionWriter.cpp -g
	g++ -c -o $(ricode)/Document.o $(ricode)/Document.cpp -g

pesquisa: $(ricode)
	g++ -c -o pesquisa.o pesquisa.cpp -I $(ricode) -g
	g++ -c -o colecao.o colecao.cpp -I $(ricode) -g
	g++ -c -o util.o util.cpp -I $(ricode) -g
	g++ -c -o le.o le.cpp -I $(ricode) -g
	g++ -c -o escreve.o escreve.cpp -I $(ricode) -g
	g++ -c -o ordena.o ordena.cpp -I $(ricode) -g
	g++ -o pesquisa pesquisa.o colecao.o util.o le.o escreve.o ordena.o $(ricode)/CollectionReader.o $(ricode)/Document.o -lz -lhtmlcxx -g 

#compila ricode
index : $(ricode)
	g++ -c -o index.o index.cpp -I $(ricode) -g
	g++ -c -o colecao.o colecao.cpp -I $(ricode) -g
	g++ -c -o util.o util.cpp -I $(ricode) -g
	g++ -c -o le.o le.cpp -I $(ricode) -g
	g++ -c -o escreve.o escreve.cpp -I $(ricode) -g
	g++ -c -o ordena.o ordena.cpp -I $(ricode) -g
	g++ -o index index.o colecao.o util.o le.o escreve.o ordena.o $(ricode)/CollectionReader.o $(ricode)/Document.o -lz -lhtmlcxx -g


clean :
	rm *.o
	rm pesquisa
	rm index
	rm index.bin*
	rm voc.txt

all : ziplib index pesquisa

teste : $(ridata) 
	{ time ./index $(ridata) $(riindex) -c ; } 2> tempo_index_compacta.txt
	{ time ./pesquisa $(ridata) $(riindex) -c < palavras.txt > resultado_compacta.txt ; } 2> tempo_pesquisa_compacta.txt
	rm index.bin* voc.txt
	{ time ./index $(ridata) $(riindex) ; } 2> tempo_index_normal.txt
	{ time ./pesquisa $(ridata) $(riindex)  < palavras.txt > resultado_normal.txt ; } 2> tempo_pesquisa_normal.txt
