ricode = ../code/src/

ziplib : $(ricode) 
	g++ -c -o $(ricode)/CollectionReader.o $(ricode)/CollectionReader.cpp -g
	g++ -c -o $(ricode)/CollectionWriter.o $(ricode)/CollectionWriter.cpp -g
	g++ -c -o $(ricode)/Document.o $(ricode)/Document.cpp -g

#compila ricode
iterate : $(ricode)
	g++ -c -o index.o index.cpp -I $(ricode) -g
	g++ -c -o colecao.o colecao.cpp -I $(ricode) -g
	g++ -c -o util.o util.cpp -I $(ricode) -g
	g++ -o index index.o colecao.o util.o $(ricode)/CollectionReader.o $(ricode)/Document.o -lz -lhtmlcxx -g

clean :
	rm *.o
	rm $(ricode)/*.o
	rm index

all : ziplib iterate

run :
	./index /Users/evelinamorim/Dropbox/UFMG/2014-1/RI/tp1/toyExample/ indexToCompressedColection.txt
