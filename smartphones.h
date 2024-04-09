#ifndef SMARTPHONES_H_INCLUDED
#define SMARTPHONES_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>

typedef struct Smartphones{

    int num_fabricacao; // numero geral para todos os objetos fabricados pela empresa
    char nome[100]; // numero que identifica de qual tipo é o smartphone fabricado e sua identificação dentro daquela especificação (ex.: SA00001, SA00002, SC30020)
    char num_serie[7];
    char data_fabricacao[11];
    int valor;


}TSmart;

TSmart *smartphone(int numf, char *nome, char *nums, char *dataf, int val);
void imprime_smart(TSmart *smart);
void salva_smart(TSmart *smart, FILE *in);
void shuffle_smart(TSmart* array[], int size);
TSmart *le_smart(FILE *in);
int tamanho_smart();
void CriarBaseDeDadosSmartphones(int tam, FILE *in);
void le_base_smart(FILE *in);
TSmart* busca_sequencial_smart(FILE *in, int tam, int numf, int *count);
void insertion_sort_disco_smart(FILE *in, int tam);
TSmart* busca_binaria_smart(int numf, FILE *in, int tam);
int sizeFile_smart(FILE *file, int contSizeFile);
int allVetFrozen_smart(int vet[6]);
int substitutionSelection_smart(FILE *file, char nameFilePartition[]);
void merge_smart(FILE *inputFiles[], int numInputFiles, FILE *outputFile);
void optimalMergeSort_smart(char *partitionPrefix, int numPartitions, int f);
#endif // SMARTPHONES_H_INCLUDED
