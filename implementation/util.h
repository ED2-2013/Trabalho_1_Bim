#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void imprima_arquivo(char *path);

int gere_arquivo_aleatorio(char *path, int n);

void filecat(FILE * destination, FILE * source);

void filencat(FILE * destination, FILE * source, int qtd, int long sourceOffset);

char * itoa_lock (int num, int lock);

void resetFile(char * path);

int count_int_arquivo(FILE * temp);

int determine_balde(int n);

void createTemporaryFiles();

void showTemporaryFiles();

void concatTempFiles(char * path);
