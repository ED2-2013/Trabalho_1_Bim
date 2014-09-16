#include "../insertion.h"

#define MAX 16
#define DIR "data/temp/"

/*
Entrada: O caminho do arquivo temporário e o numero para ser inserido
Saída: Insere um numero na posição correta do arquivo
Custo : O(m) - Quantidade de elementos no arquivo temporário
*/
void insertion(char * path, int num){

    FILE * temp = fopen(path, "rb+");

    int size = count_int_arquivo(temp), * buffer = (int *) malloc(MAX*sizeof(int)), i, qtd, index, auxSize;

    if(!size)
        fwrite(&num,sizeof(int),1,temp);
    else {
        auxSize = size;
        index = 0;
        rewind(temp);
        while(index <= size && auxSize > 0){
            qtd = (auxSize > MAX) ? MAX : auxSize;
            fread(buffer,sizeof(int),qtd,temp);
            for(i=0; i < qtd; i++){
                if(num<buffer[i]){
                    put(path,num,index);
                    fclose(temp);
                    return;
                }
                index++;
            }
            auxSize -= qtd;
        }
        fseek(temp,0L,SEEK_END);
        fwrite(&num,sizeof(int),1,temp);
    }
    fclose(temp);
}

/*
Entrada: O caminho do arquivo temporário, número para ser inserido e o index para inserir na posição correta.
Saída: Insere o número na posição correta do arquivo temporário.
Custo : O(m) - Que representa a quantidade de elementos no arquivo temporário
*/
void put( char * path, int num, int index){

    char * pathAux = (char *) malloc(strlen(DIR)*sizeof(char));
    strcat(strcpy(pathAux,DIR),"aux");

    resetFile(pathAux);

    FILE * aux = fopen(pathAux,"rb+"), * temp = fopen(path,"rb+");
    rewind(aux);
    rewind(temp);

    if(!index){
        fwrite(&num,sizeof(int),1,aux);
        filecat(aux,temp);
    } else {
        filencat(aux,temp,index,0);
        fwrite(&num,sizeof(int),1,aux);
        filencat(aux,temp,count_int_arquivo(temp)-index,(long)(index*sizeof(int)));
    }
    resetFile(path);
    rewind(temp);
    rewind(aux);

    filecat(temp,aux);

    fclose(aux);
    fclose(temp);
}
