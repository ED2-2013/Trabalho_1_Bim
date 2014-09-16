#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "bucket.h"
#include "insertion.h"

int main(int argc, char **argv){

    gere_arquivo_aleatorio("data/teste",10000);

    printf("Entrada : \n");
    imprima_arquivo("data/teste");

    bucket_sort(fopen("data/teste","r+"));

    printf("\n\nSaída : \n");
    imprima_arquivo("data/fteste");

	return 0;
}
