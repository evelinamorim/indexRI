ricode = ../code/src/

ziplib : $(ricode) 
	g++ -c -o $(ricode)/CollectionReader.o $(ricode)/CollectionReader.cpp
	g++ -c -o $(ricode)/CollectionWriter.o $(ricode)/CollectionWriter.cpp
	g++ -c -o $(ricode)/Document.o $(ricode)/Document.cpp

#compila ricode
iterate : $(ricode)
	g++ -c -o documento.o documento.cpp -I $(ricode)
	g++ -o documento documento.o $(ricode)/CollectionReader.o $(ricode)/Document.o -lz

clean:
	rm *.o
	rm $(ricode)/*.o

all : ziplib iterate
