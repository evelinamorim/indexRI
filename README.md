1. Compilação

Antes de compilar pode ser necessário inicializar algumas variáveis para 
construção do índice e para a realização da pesquisa. 

Após as configurações  para compilar o código fonte do índice basta digitar 
no terminal:

> make ziplib
e depois
> make index

Para compilar o código fonte da pesquisa basta digitar:

> make ziplib
e depois
> make pesquisa

Caso queira compilar pesquisa e índice de uma vez só, basta digitar no 
terminal:

> make all

A configuração  para a compilação do índice consiste em três partes: índice, pesquisa e makefile.  Nas subseções a seguir a configuração necessária para a compilação 
de cada um dos itens listados estão detalhados.


1.1 Configurar Índice

A construção do índice gera dois arquivos: o arquivo de índice e o arquivo de vocabulário. A atribuição de tais nomes fica no arquivo coleca.cpp. No início de colecao.cpp existem duas variaveis que armazenam respectivamente o nome do arquivo de índice e o nome do arquivo de vocabulário. Segue como está o nome padrão em colecao.cpp:

const string Colecao::nome_arquivo_indice="index.bin";
const string Colecao::nome_arquivo_vocabulario="voc.txt";

1.2 Configurar Pesquisa

A execução da pesquisa necessita de dois arquivos: o arquivo de índice e o arquivo de vocabulário. A atribuição de tais nomes fica no arquivo pesquisa.cpp. No início de pesquisa.cpp existem duas variaveis que armazenam respectivamente o nome do arquivo de índice e o nome do arquivo de vocabulário. Segue como está o nome padrão em pesqisa.cpp:

const string Colecao::nome_arquivo_indice="index.bin";
const string Colecao::nome_arquivo_vocabulario="voc.txt";

1.3 Configurar Makefile

Três variáveis podem ser modificadas no arquivo de Makefile.

a) ricode: diretório que contém o código da biblioteca que le arquivos compactados. 
Neste diretório devem estar os códigos fontes CollectionReader.cpp, CollectionWriter.cpp e Document.cpp.

b) ridata: É o diretorio onde se encontra o arquivo de índice e o arquivo de dados.

c) riindex: nome do arquivo com a listagem de arquivos a serem indexados.


2. Execução

A execução deste trabalho deve ser feita em duas partes. Primeiro 
compilar os fontes da pesquisa e do índice como explicado na seção anterior 
, depois executar a construção do índice e depois executar a pesquisa.

2.1. Rodando alguns testes

Os principais testes são executados durante a pesquisa. No entanto, 
para isso o índice deve ser construído. Seguem nas próximas subseções 
alguns detalhes.

2.1.1 Índice

A compilação do índice gerou um arquivo executável de nome "index". Este 
executável pode construir dois tipos de índice: comprimido e não comprimido.

Para gerar o índice não comprimido basta executar no terminal:

./index DIRETORIO_DE_PAGINAS ARQUIVO_PARA_PAGINAS

A geração de índice comprimido é similar a execução do índice não comprimido. No 
entanto o usuário deve acrescentar a opção -c ao final do comando. Como a seguir:

./index DIRETORIO_DE_PAGINAS ARQUIVO_PARA_PAGINAS -c

ATENÇÃO: para gerar um novo índice apague o arquivo de índice anterior e o 
arquivo de vocabulário também. 

2.1.2 Pesquisa

A compilação do código fonte da pesquisa gerou um arquivo executável 
de nome "pesquisa". Assim como o índice a pesquisa precisa do diretório de 
páginas e do arquivo de índice para as páginas para poder executar. Veja 
a sequir um exemplo:

./pesquisa DIRETORIO_DE_PAGINAS ARQUIVO_PARA_PAGINAS

A entrada de consultas para pesquisa é feita via entrada padrão. Após digitar 
a consulta desejada o usuário pressionar ENTER que a pesquisa é realizada. 

Após a exibição dos resultados o programa aguarda nova consulta do usuário. Para 
interromper o programa é necessário o usuário enviar um sinal de EOF ou Ctrl-C.

2.3 Testes prontos (make teste)

Existe um conjunto de palavras para pesquisa no arquivo palavras.txt. O 
usuário pode modificar este conjunto de palavras de teste.

Neste conjunto de testes prontos inicialmente é construído arquivos de índice 
compactado e a lista de vocábulos correspondentes a este índice. Após 
esta construção é feita uma pesquisa com o conjunto de palavras em palavras.txt 
e o resultado é colocado em resultado_compacta.txt.

Após o teste em um índice compactado o script apaga o índice e o vocabulario 
criados. Então é criado um índice não compactado. Finalmente são realizadas 
as consultas contidas em palavras.txt e o resultado destas pesquisas é 
colocada em resultado_normal.txt.


