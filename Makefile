ricode = ../code/src/

ziplib : $(ricode) 
	g++ -c -o $(ricode)/CollectionReader.o $(ricode)/CollectionReader.cpp -g
	g++ -c -o $(ricode)/CollectionWriter.o $(ricode)/CollectionWriter.cpp -g
	g++ -c -o $(ricode)/Document.o $(ricode)/Document.cpp -g
le:
	g++ -c -o util.o util.cpp -I $(ricode) -g
	g++ -c -o le.o le.cpp -I $(ricode) -g
	g++ -o le le.o util.o -g
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
	rm $(ricode)/*.o
	rm index
	rm index.bin*

all : ziplib index

run :
	./index /Users/evelinamorim/Dropbox/UFMG/2014-1/RI/tp1/toyExample/ indexToCompressedColection.txt
