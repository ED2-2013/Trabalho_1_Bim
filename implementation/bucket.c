#include "bucket.h"

#define MAX 16
#define LOCK 2
#define DIR "data/temp/"
#define BUCKET_INTERVAL 100
#define TOTAL_INTERVAL 1000
/*
Entrada: Executa o BucketSort separando os números nos seus respectivos baldes, ordenando-os e concatenando eles num arquivo final.
Saída: Concatena os arquivos temporários gerando o arquivo ordenado
Custo : O(n²) - Percorre todos os números, e depois compara com todos os números novamente.
*/
void bucket_sort(FILE * arquivo){

    int * buffer = (int *) malloc(MAX*sizeof(int));

    createTemporaryFiles();

    while(fread(buffer, sizeof(int), MAX, arquivo)){
        coloca_balde(buffer);
    }

    concatTempFiles("data/fteste");

}

/*
Entrada: Recebe um ponteiro de inteiro que é o buffer atual para ser distribuído entre os arquivos temporários
Saída: Os números inseridos nos seus respectivos baldes.
Custo : O(m) - Percorre cada elemento do arquivo original, e depois compara com cada elemento do arquivo temporário
*/
void coloca_balde(int * buffer){

    int i;
    char * path = (char *) malloc(strlen(DIR)*sizeof(char));

		for(i=0; i<MAX; i++){
		    strcat(strcpy(path,DIR),itoa_lock(determine_balde(buffer[i]),LOCK));
            insertion(path,buffer[i]);
        }
}
