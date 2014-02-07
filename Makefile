ricode = ../code/src/

ziplib : $(ricode) 
	g++ -c -o $(ricode)/CollectionReader.o $(ricode)/CollectionReader.cpp
	g++ -c -o $(ricode)/CollectionWriter.o $(ricode)/CollectionWriter.cpp
	g++ -c -o $(ricode)/Document.o $(ricode)/Document.cpp

#compila ricode
iterate : $(ricode)
	g++ -c -o index.o index.cpp -I $(ricode)
	g++ -c -o colecao.o colecao.cpp -I $(ricode)
	g++ -o index index.o colecao.o $(ricode)/CollectionReader.o $(ricode)/Document.o -lz

clean :
	rm *.o
	rm $(ricode)/*.o
	rm index

all : ziplib iterate

run :
	./index /Users/evelinamorim/Dropbox/UFMG/2014-1/RI/tp1/toyExample/ indexToCompressedColection.txt
