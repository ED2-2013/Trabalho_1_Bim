#include "../util.h"

#define MAX 16
#define BUCKET_INTERVAL 100
#define TOTAL_INTERVAL 1000
#define DIR "data/temp/"
#define LOCK 2

/*Geração de arquivos binários. Utilize para criar arquivos de teste.
Entrada:
	1.Nome do arquivo a ser gerado
	2.quantidade de números no arquivo (a função vai arredondar um multiplo de 16)
Saída:
	Um arquivo binário com numeros entre 0 e 999
Retorno:
	Quantidade de números no arquivo
	Custo : O(n), pois as outras variáveis são constantes
*/
int gere_arquivo_aleatorio(char *path, int n)
{
    FILE *arquivo = fopen(path, "wb+");
    int *buffer, qtd, i;

    buffer = (int*)malloc(MAX*sizeof(int));

    n = n - n%MAX;//subtrai o resto para arredondar

    qtd = n/MAX;

    while(qtd > 0){
    	for(i=0; i<MAX; i++){
    		buffer[i] = random()%TOTAL_INTERVAL;
    	}
    	fwrite(buffer, sizeof(int), MAX, arquivo);
    	qtd--;
    }

    fclose(arquivo);

    return n;
}

/*
Função que imprime arquivos binários contendo inteiros, buffer por buffer
Entrada:	path do arquivo
Custo : O(n) - Pois as outras variáveis são constantes.
*/
void imprima_arquivo(char *path)
{
    int *buffer, i, p;
    FILE *arquivo;
    buffer = (int*)malloc(16*sizeof(int));
    arquivo = fopen(path, "rb+");
    while(1)
    {
        for(i=0; i<16; i++)
            buffer[i] = 0;

        p=fread(buffer, sizeof(int), 16, arquivo);
        if(p==0)break;//Se não tiver mais o que ser lido, sai
        for(i=0; i<16; i++)
        {
            printf("%3d(%2d) ", buffer[i], determine_balde(buffer[i]));

        }
        printf("\n");
    }
    fclose(arquivo);
}

/*
Entrada: O arquivo fonte e o arquivo destino
Saída: Concatena o arquivo fonte no arquivo destino
Custo : O(m) - Números contidos no arquivo temporário fonte
*/
void filecat(FILE * destination, FILE * source)
{
    filencat(destination,source, count_int_arquivo(source), 0);
}

/*
Entrada: O arquivo fonte e o arquivo destino e a quantidade de elementos a serem copiados e o deslocamento do arquivo fonte
Saída: Concatena o arquivo fonte no arquivo destino
Custo : O(i) - Quantidade de arquivos passados como parâmetro para copiar
*/
void filencat(FILE * destination, FILE * source, int qtd, int long sourceOffset) {

    rewind(source);

    fseek(source,sourceOffset,SEEK_SET);

    int * buffer = (int *)malloc(MAX*sizeof(int));
    while(qtd > 0) {
        if(qtd<MAX) {
            fread(buffer, sizeof(int), qtd, source);
            fwrite(buffer, sizeof(int), qtd, destination);
            qtd -= qtd;
        } else {
            fread(buffer, sizeof(int), MAX, source);
            fwrite(buffer, sizeof(int), MAX, destination);
            qtd -= MAX;
        }
    }
}

/*
Entrada: Número inteiro e quantidade de casas decimais que deseja retornar a partir do número de entrada. 
Saída: Retorna o char do número com a quantidade de casas decimais pedida.
Retorno: Se o número de entrada possuir menos casas decimais do que a quantidade de casas decimais que for exigido como saída, retorna NULL. Caso contrário retorna o char do número com a quantidade de casas decimais exigidas. Exemplo: Se int num = 1 e int lock = 2, o retorno será char c = 01. 
Custo : O(1)
*/
char * itoa_lock (int num, int lock)
{

    if( (num < 0) || (num > pow(10,lock)) )
        return NULL;

    char * array = NULL;
    int i, divisor, maiorDecimal;

    array = (char *) malloc(lock+1);

    for(i = 0; i<lock ; i++)
    {
        divisor = pow(10,lock - i - 1);
        maiorDecimal = floor(num/divisor);

        array[i] = (int)floor(num/divisor)+48;
        num -= maiorDecimal * divisor;
    }
    array[lock] = '\0';

    return array;
}

/*
Entrada: O caminho temporário do arquivo a ser limpo
Saída: Limpa o arquivo
Custo : O(1)
*/
void resetFile(char * path)
{
    fclose(fopen(path, "wb+"));
}

/*
Entrada: O arquivo que deve-se saber a quantidade de elementos contidos nele
Saída: Retorna a quantidade de elementos no arquivo
Retorno: Quantidade de números
Custo : O(1)
*/
int count_int_arquivo(FILE * temp)
{

    int count;
    FILE * aux = temp;

    fseek(aux, 0L, SEEK_END);
    count = ftell(aux)/sizeof(int);

    return count;
}

/*
Entrada: Número inteiro.
Saída: Qual balde o número inteiro deve ser inserido.
Retorno: Número inteiro que representa o número do balde
Custo : O(1)
*/
int determine_balde(int n)
{
    return floor(n/BUCKET_INTERVAL);
}

/*
Saída: Gera os arquivos temporários que serão utilizados pelo BucketSort
Custo : O(1) Todas as variáveis são constantes
*/
void createTemporaryFiles()
{
    int i;

    char * path = (char *) malloc(strlen(DIR)*sizeof(char));

    for(i=0; i < TOTAL_INTERVAL/BUCKET_INTERVAL ; i++)
    {
        strcat(strcpy(path,DIR),itoa_lock(i,LOCK));
        resetFile(path);
    }
}

/*
Saída: Mostra o que está contido no arquivo temporário
Custo : O(n) Pois imprime os números de todos os arquivos temporários
*/
void showTemporaryFiles()
{
    int i;

    char * path = (char *) malloc(strlen(DIR)*sizeof(char));

    for(i=0; i < TOTAL_INTERVAL/BUCKET_INTERVAL ; i++)
    {
        strcat(strcpy(path,DIR),itoa_lock(i,LOCK));

        printf("Arquivo Temp : %s\n",itoa_lock(i,LOCK));
        imprima_arquivo(path);
        printf("\n\n");
    }
}

/*
Entrada: Caminho do arquivo a ser gerado com os arquivos concatenados
Saída: Gera o arquivo final ordenado
Custo : O(n) Concatena os arquivos temporários, logo, n
*/
void concatTempFiles(char * path)
{
    resetFile(path);

    int i;
    FILE * concatFile = fopen(path,"ab+"), *temp;
    char * pathTemp = (char *) malloc(strlen(DIR)*sizeof(char));

    for(i=0; i < TOTAL_INTERVAL/BUCKET_INTERVAL ; i++)
    {
        strcat(strcpy(pathTemp,DIR),itoa_lock(i,LOCK));
        temp = fopen(pathTemp,"rb");
        filecat(concatFile,temp);
        fclose(temp);
    }
    fclose(concatFile);
}
