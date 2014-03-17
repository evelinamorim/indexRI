ricode = ../code/src/
ridata = /Users/evelinamorim/Documents/Ufmg/2014-1/RI/
riindex = index.txt

ziplib : $(ricode) 
	g++ -O3 -c -o $(ricode)/CollectionReader.o $(ricode)/CollectionReader.cpp -ggdb
	g++ -O3  -c -o $(ricode)/CollectionWriter.o $(ricode)/CollectionWriter.cpp -ggdb
	g++ -O3  -c -o $(ricode)/Document.o $(ricode)/Document.cpp -ggdb

pesquisa: $(ricode)
	g++ -O0  -c -o pesquisa.o pesquisa.cpp -I $(ricode) -g
	g++ -O0  -c -o colecao.o colecao.cpp -I $(ricode) -g
	g++ -O0  -c -o util.o util.cpp -I $(ricode) -g
	g++ -O0  -c -o le.o le.cpp -I $(ricode) -g
	g++ -O0  -c -o escreve.o escreve.cpp -I $(ricode) -g
	g++ -O0  -c -o ordena.o ordena.cpp -I $(ricode) -g
	g++ -O0  -o pesquisa pesquisa.o colecao.o util.o le.o escreve.o ordena.o $(ricode)/CollectionReader.o $(ricode)/Document.o -lz -lhtmlcxx -g 

#compila ricode
index : $(ricode)
	g++ -O3 -c -o index.o index.cpp -I $(ricode) -ggdb
	g++ -O3  -c -o colecao.o colecao.cpp -I $(ricode) -ggdb
	g++ -O3  -c -o util.o util.cpp -I $(ricode) -ggdb
	g++ -O3  -c -o le.o le.cpp -I $(ricode) -ggdb
	g++ -O3  -c -o escreve.o escreve.cpp -I $(ricode) -ggdb
	g++ -O3  -c -o ordena.o ordena.cpp -I $(ricode) -ggdb
	g++ -O3  -o index index.o colecao.o util.o le.o escreve.o ordena.o $(ricode)/CollectionReader.o $(ricode)/Document.o -lz -lhtmlcxx -ggdb

index1 : $(ricode)
	g++ -O3 -c -o index.o index.cpp -I $(ricode) -ggdb
	g++ -O3  -c -o colecao.o colecao.cpp -I $(ricode) -ggdb
	g++ -O3  -c -o util.o util.cpp -I $(ricode) -ggdb
	g++ -O3  -c -o le.o le.cpp -I $(ricode) -ggdb
	g++ -O3  -c -o escreve.o escreve.cpp -I $(ricode) -ggdb
	g++ -O3  -c -o ordena.o ordena.cpp -I $(ricode) -ggdb
	g++ -O3  -o index1 index.o colecao.o util.o le.o escreve.o ordena.o $(ricode)/CollectionReader.o $(ricode)/Document.o -lz -lhtmlcxx -ggdb
clean :
	rm *.o
	rm pesquisa
	rm index
	rm index.bin*
	rm voc.txt

all : ziplib index pesquisa

teste : $(ridata) 
	{ time ./index $(ridata) $(riindex) -c ; } 2>> tempo_index_compacta.txt}
	rm index.txt voc.txt
	{ time ./index $(ridata) $(riindex) -c ; } 2>> tempo_index_compacta.txt
	rm index.txt voc.txt
	{ time ./index $(ridata) $(riindex) -c ; } 2>> tempo_index_compacta.txt
	#{ time ./pesquisa $(ridata) $(riindex) -c < palavras.txt > resultado_compacta.txt ; } 2> tempo_pesquisa_compacta.txt
