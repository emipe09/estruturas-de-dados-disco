#ifndef COMPUTADORES_H_INCLUDED
#define COMPUTADORES_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>

typedef struct Computadores{

    int num_fabricacao; // numero geral para todos os objetos fabricados pela empresa
    char nome[100];
    char num_serie[7]; // numero que identifica de qual tipo é o computador fabricado e sua identificação dentro daquela especificação (ex.: CA00001, CA00002, CC30020)
    char data_fabricacao[11];
    int valor;
    struct Computer *next;

}TComp;


typedef struct HashTable {
    int size;
    TComp **table;
} HashTable;

//funções em computadores.c
TComp *computador(int numf, char *nome, char *nums, char *dataf, int val);
void imprime(TComp *comp);
void salva(TComp *comp, FILE *in);
void shuffle(TComp* array[], int size);
TComp *le(FILE *in);
int tamanho();
void CriarBaseDeDadosComputadores(int tam, FILE *in);
void le_base_comp(FILE *in);
TComp* busca_sequencial_comp(FILE *in, int tam, int numf, int *count);
void insertion_sort_disco_comp(FILE *arq, int tam);
TComp* busca_binaria_comp(int numf, FILE *in, int tam);
int substitutionSelection(FILE *file, char nameFilePartition[]);
void merge(FILE *inputFiles[], int numInputFiles, FILE *outputFile);
void optimalMergeSort(char *partitionPrefix, int numPartitions, int f);
void hashTable(FILE *file, int numberOfPartition, int sizeFile);
TComp *searchHashTable(int computerID, int partitionID);
void deleteHashTable(int computerID, int partitionID);
void insertHashTable(TComp *compHashTableInsert, int partitionID);


#endif // COMPUTADORES_H_INCLUDED
