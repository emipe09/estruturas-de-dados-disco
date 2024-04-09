#include "smartphones.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>

#define MAX_STR_SIZE	30
#define MIN_STR_SIZE	5

#define F 4

int y;//armazena o tamanho da base
//cria um smartphone
TSmart *smartphone(int numf, char *nome, char *nums, char *dataf, int val){

    TSmart *smart = (TSmart*) malloc(sizeof(TSmart));
    if (smart) memset(smart, 0, sizeof(TSmart));
    smart->num_fabricacao = numf;
    strcpy(smart->nome, nome);
    strcpy(smart->num_serie, nums);
    strcpy(smart->data_fabricacao, dataf);
    smart->valor=val;
    return smart;

}

void imprime_smart(TSmart *smart){

    printf("\nNúmero de Fabricação: %i", smart->num_fabricacao);
    printf("\nNome do Smartphone: %s", smart->nome);
    printf("Número de Série: %s", smart->num_serie);
    printf("\nData de Fabricação: %s", smart->data_fabricacao);
    printf("\nValor: %i", smart->valor);
    printf("\n**********************************************\n");

}

//vai salvar no arquivo na posição atual em que se encontra o cursor
void salva_smart(TSmart *smart, FILE *in)
{
    fwrite(&smart->num_fabricacao, sizeof(int), 1, in);
    fwrite(smart->nome, sizeof(char), sizeof(smart->nome), in);
    fwrite(smart->num_serie, sizeof(char), sizeof(smart->num_serie), in);
    fwrite(smart->data_fabricacao, sizeof(char), sizeof(smart->data_fabricacao), in);
    fwrite(&smart->valor, sizeof(int), 1, in);

}

void shuffle_smart(TSmart* array[], int size)
{
    srand(time(NULL));
    for (int i = size - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        TSmart* smart = array[i];
        array[i] = array[j];
        array[j] = smart;
    }
}

TSmart *le_smart(FILE *in)
{
    TSmart *smart = (TSmart *) malloc(sizeof(TSmart));
    if (0 >= fread(&smart->num_fabricacao, sizeof(int), 1, in))
    {
        free(smart);
        return NULL;
    }
    fread(smart->nome, sizeof(char), sizeof(smart->nome), in);
    fread(smart->num_serie, sizeof(char), sizeof(smart->num_serie), in);
    fread(smart->data_fabricacao, sizeof(char), sizeof(smart->data_fabricacao), in);
    fread(&smart->valor, sizeof(int), 1, in);
    return smart;
}

//tamanho em bytes do smartphone
int tamanho_smart()
{
    return sizeof(int)  //cod
           + sizeof(char) * 100 //nome
           + sizeof(char) * 7 //numero de serie
           + sizeof(char) * 11//data de fabricacao
           +sizeof(int); //valor do computador

}
int tam_smart(){
    return y;
}
void CriarBaseDeDadosSmartphones(int tam, FILE *in)
{
    y=tam;
    int ordem[tam];
    for(int i = 0; i<tam;i++)
    {
        ordem[i]=i+1;

    }
    shuffle(&ordem, tam);
    TSmart *smart = (TSmart *)malloc(sizeof(TSmart));
    char nome[100], nums[7], dataf[11];
    int val;
    for(int i = 0; i<tam; i++)
    {
//        printf("\nDigite o nome do smartphone: ");
//        fflush(stdin);
//        fgets(nome, sizeof(nome), stdin);
//
//        printf("\nDigite o número de série do smartphone(2 letras, 4 números): ");
//        fflush(stdin);
//        fgets(nums, sizeof(nums), stdin);
//
//        printf("\nDigite a data de fabricação(DD/MM/AAAA): ");
//        fflush(stdin);
//        fgets(dataf, sizeof(dataf), stdin);
//
//        printf("\nDigite o valor em reais do smartphone: ");
//        scanf("%i", &val);
//
//        smart = smartphone(ordem[i], nome, nums, dataf, val);
        smart = smartphone(ordem[i], "Cel", "CE0000", "20/04/2023", 3000);
        salva_smart(smart, in);
    }
    free(smart);

}

void le_base_smart(FILE *in)
{
    printf("\n\nSMARTPHONES...\n\n");
    rewind(in);
    TSmart *s;
    while ((s = le_smart(in)) != NULL)
    {
        imprime(s);
        free(s);
    }
}

TSmart* busca_sequencial_smart(FILE *in, int tam, int numf, int *count)
{
    clock_t inicio;
    inicio= clock();
    rewind(in);
    TSmart *smart;
    while ((smart = le(in)) != NULL)
    {
        if(smart->num_fabricacao==numf)
        {
            clock_t end;
            end=clock();
            printf("\nNumero de comparaçoes: %d",(*count)+1);
            printf("\nTempo de Execucao:  %f\n", ((float)(end-inicio))/CLOCKS_PER_SEC);
            return smart;

        }
        (*count)++;
    }
    clock_t end;
    end=clock();
    printf("\nNumero de comparaçoes: %d",(*count)+1);
    printf("\nTempo de Execucao:  %f\n", ((float)(end-inicio))/CLOCKS_PER_SEC);
    return NULL;
}
void insertion_sort_disco_smart(FILE *in, int tam)
{
    clock_t inicio=clock();
    int i, count=0;
    for (int j = 2; j <= tam; j++)
    {
        fseek(in, (j-1) * tamanho_smart(), SEEK_SET);
        TSmart *smartj = le_smart(in);
        i = j - 1;
        fseek(in, (i-1) * tamanho_smart(), SEEK_SET);
        TSmart *smarti = le_smart(in);
        while ((i > 0) && (smarti->num_fabricacao > smartj->num_fabricacao))
        {
            fseek(in, i * tamanho_smart(), SEEK_SET);
            salva_smart(smarti, in);
            i = i - 1;
            count++;
            if (i > 0)
            {
                fseek(in, (i-1) * tamanho_smart(), SEEK_SET);
                smarti = le_smart(in);
            }
        }
        fseek(in, (i) * tamanho_smart(), SEEK_SET);
        salva_smart(smartj, in);
    }
    fflush(in);
    le_base_smart(in);
    clock_t end= clock();
    printf("Número de comparações: %i", count);
}

TSmart* busca_binaria_smart(int numf, FILE *in, int tam)
{

    int left = 0, right = tam - 1, comps=0;

    while(left <= right)
    {
        int middle = (left + right) / 2;
        fseek(in, middle * tamanho_smart(), SEEK_SET);
        TSmart* smart = le_smart(in);
        if(numf == smart->num_fabricacao)
        {
            clock_t end = clock();
            printf("\nNumero de Comparacoes por Busca Binaria: %d ",comps+1);
            //printf("\nTempo de Execucao:  %f\n", ((float)(end-inicio))/CLOCKS_PER_SEC);
            return smart;
        }
        else if(smart->num_fabricacao < numf)
        {
            left = middle + 1;
        }
        else
        {
            right = middle - 1;
        }
        comps++;
    }


    return NULL;
}


int sizeFile_smart(FILE *file, int contSizeFile)
{
    fseek(file, 0, SEEK_SET);
    while (!feof(file))
    {
        fseek(file, contSizeFile * tamanho_smart(), SEEK_SET);
        TSmart *aux = le_smart(file);
        if (aux != NULL)
        {
            contSizeFile++;
        }
    }

    return contSizeFile;
}

void printPartitionSmartphoneID(FILE *file, char partitionName[])
{
    printf("\nIDS dos computadores por particao %s: \n --->  ", partitionName);

    for (int i = 0; i < sizeFile_smart(file, 0); ++i)
    {
        fseek(file, i * tamanho_smart(), SEEK_SET);
        TSmart *aux = le_smart(file);
        printf(" %i ", aux->num_fabricacao);
    }

    printf("\n");
}

int allVetFrozen_smart(int vet[6])
{
    int cont = 0;
    for (int i = 0; i < 6; ++i)
    {
        if (vet[i] == 1)
        {
            cont++;
        }
    }

    if (cont == 6)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


int substitutionSelection_smart(FILE *file, char nameFilePartition[])
{
    int numberOfPartition = 0, contSizeFile = 0, position = 6, smallElementPosition = 0, smallElement = 999999, sizeFileAux = 0, selectedPosition = 0;
    TSmart smart[6];
    int auxVetSmart[6] = {0, 0, 0, 0, 0, 0};

    rewind(file);

    sizeFileAux = sizeFile_smart(file, contSizeFile);

    printf("\nPerforming substitution selection...");

    for (int i = 0; i < 6; ++i)
    {
        fseek(file, i * tamanho_smart(), SEEK_SET);
        TSmart *aux = le_smart(file);
        printf("%i\n", aux->num_fabricacao);
        smart[i] = *aux;
    }

    for (int i = 0; i < 6; ++i)
    {
        auxVetSmart[i] = smart[i].num_fabricacao;
    }

    while (position < sizeFileAux)
    {
        char partitionName[100];
        char str1[100];
        char str2[100] = ".dat";

        snprintf(str1, 100, "%d", numberOfPartition);

        // Concatenate the name of the partition file
        strcpy(partitionName, nameFilePartition);
        strcat(partitionName, str1);
        strcat(partitionName, str2);

        FILE *filePartition = fopen(partitionName, "wb+");

        int auxVetFrozen[6] = {0, 0, 0, 0, 0, 0,};

        while (position < sizeFileAux)
        {
            smallElement = 9999999;

            for (int i = 0; i < 6; ++i)
            {
                if (smallElement > auxVetSmart[i] && auxVetFrozen[i] != 1)
                {
                    smallElement = auxVetSmart[i];
                    smallElementPosition = i;
                }
            }

            salva(&smart[smallElementPosition], filePartition);

            fseek(file, position * tamanho_smart(), SEEK_SET);
            TSmart *aux = le_smart(file);

            position++;
            auxVetSmart[smallElementPosition] = aux->num_fabricacao;
            smart[smallElementPosition] = *aux;

            if (aux->num_fabricacao < smallElement)
            {
                auxVetFrozen[smallElementPosition] = 1;
            }

            if (allVetFrozen_smart(auxVetFrozen) == 1)
            {
                numberOfPartition++;
                break;
            }
        }

        fclose(filePartition);

        if (position >= sizeFileAux)
        {
            // Include the remaining elements in the last partition and keep it sorted
            int lastPartitionIndex = numberOfPartition;
            for (int i = 0; i < 6; ++i)
            {
                if (auxVetFrozen[i] == 0)
                {
                    // Read the current record from the partition
                    TSmart *currentRecord = &smart[i];

                    // Open the last partition file for reading
                    char partitionNameLast[100];
                    snprintf(partitionNameLast, sizeof(partitionNameLast), "%s%d.dat", nameFilePartition, lastPartitionIndex);
                    FILE *filePartitionLast = fopen(partitionNameLast, "rb+");

                    // Create a temporary array to hold the records from the last partition
                    TSmart lastPartitionRecords[6];
                    int lastPartitionSize = 0;

                    // Read records from the last partition and store them in the temporary array
                    while (!feof(filePartitionLast))
                    {
                        TSmart *record = le_smart(filePartitionLast);
                        if (record != NULL)
                        {
                            lastPartitionRecords[lastPartitionSize++] = *record;
                            free(record);
                        }
                    }

                    // Close the last partition file
                    fclose(filePartitionLast);

                    // Open the last partition file again for writing
                    FILE *filePartitionLastWrite = fopen(partitionNameLast, "wb+");

                    // Insert the current record in the correct position in the temporary array
                    int insertIndex = 0;
                    while (insertIndex < lastPartitionSize && lastPartitionRecords[insertIndex].num_fabricacao < currentRecord->num_fabricacao)
                    {
                        insertIndex++;
                    }
                    // Shift records to make space for the new record
                    for (int j = lastPartitionSize; j > insertIndex; j--)
                    {
                        lastPartitionRecords[j] = lastPartitionRecords[j - 1];
                    }
                    // Insert the new record
                    lastPartitionRecords[insertIndex] = *currentRecord;

                    // Write the sorted records back to the last partition file
                    for (int j = 0; j < lastPartitionSize + 1; j++)
                    {
                        salva(&lastPartitionRecords[j], filePartitionLastWrite);
                    }

                    // Close the last partition file
                    fclose(filePartitionLastWrite);

                    // Mark the current record as added
                    auxVetFrozen[i] = 1;
                }
            }

            break;
        }
    }

    for (int i = 0; i <= numberOfPartition; ++i)
    {
        char partitionName[100];
        char str1[100];
        char str2[100] = ".dat";

        snprintf(str1, 100, "%d", i);
        strcat(strcpy(partitionName, nameFilePartition), str1);
        strcat(partitionName, str2);

        FILE *filePartition = fopen(partitionName, "rb+");
        printPartitionSmartphoneID(filePartition, partitionName);
        fclose(filePartition);
    }
    return numberOfPartition+1;
}

//realiza a intercalação de um array de particoes de tamanho F-1 em um só arquivo
void merge_smart(FILE *inputFiles[], int numInputFiles, FILE *outputFile) {
    TSmart *smartArray[numInputFiles];
    int minIndex, i;

    for (i = 0; i < numInputFiles; i++) {
        smartArray[i] = le_smart(inputFiles[i]);
    }

    while (1) {
        minIndex = -1;
        for (i = 0; i < numInputFiles; i++) {
            if (smartArray[i] != NULL) {
                if (minIndex == -1 || smartArray[i]->num_fabricacao < smartArray[minIndex]->num_fabricacao) {
                    minIndex = i;
                }
            }
        }

        if (minIndex == -1) {
            break;
        }

        salva_smart(smartArray[minIndex], outputFile);
        smartArray[minIndex] = le_smart(inputFiles[minIndex]);
    }

    for (i = 0; i < numInputFiles; i++) {
        free(smartArray[i]);
    }
}

//funcao principal da intercalação ótima, que tem como parametros o prefixo da particao, o número de partições criadas
// e um valor inteiro f que vai controlar as fases da intercalação
void optimalMergeSort_smart(char *partitionPrefix, int numPartitions, int f) {
    FILE *inputFiles[f - 1];
    char partitionName[100];
    //vai ser usado para saber o numero da ultima particao criada, que vai ser o arquivo final intercalado
    int x=numPartitions;
    //vai ser usado para controlar o numero da particao criada após F-1 fases
    int y=x;
    int numFilesToMerge=0;
    int count=0;
    //loop até que reste apenas uma particao
    while (numPartitions > 1) {
        if (numPartitions < f) {
            numFilesToMerge = numPartitions;
        } else {
            numFilesToMerge = f-1;
        }
        //preenche o array com até f-1 particoes
        for (int i = 0; i < numFilesToMerge; i++) {
            snprintf(partitionName, sizeof(partitionName), "%s%d.dat", partitionPrefix, i+count*(f-1));
            inputFiles[i] = fopen(partitionName, "rb");
            if (!inputFiles[i]) {
                perror("Error opening input file");
            }
        }
        //cria a particao de saida da fase
        snprintf(partitionName, sizeof(partitionName), "%s%d.dat", partitionPrefix, y);
        y++;
        FILE *outputFile = fopen(partitionName, "wb");
        if (!outputFile) {
            perror("Error opening output file");
            // Você pode adicionar o tratamento de erro apropriado aqui
        }
        //manda para funcao de intercalar array
        merge_smart(inputFiles, numFilesToMerge, outputFile);
        //deleta as particoes ja intercaladas na fase
        for (int i = 0; i < numFilesToMerge; i++) {
            fclose(inputFiles[i]);
            snprintf(partitionName, sizeof(partitionName), "%s%d.dat", partitionPrefix, i+count*(f-1));
            remove(partitionName); // Remove o arquivo de partição intercalado
        }
        fclose(outputFile);
        count++;
        //atualiza o numero de particoes restantes
        numPartitions = numPartitions - (numFilesToMerge - 1);
    }
    // Abre o arquivo da última partição ordenada
    snprintf(partitionName, sizeof(partitionName), "%s%d.dat", partitionPrefix, x+count-1);
    FILE *finalOutputFile = fopen(partitionName, "rb");
    if (!finalOutputFile) {
        perror("Error opening final output file");

    }

    // Imprime a última partição ordenada
    printPartitionSmartphoneID(finalOutputFile, "saida");
    fclose(finalOutputFile);
}
void hashTable_smart(FILE *file, int numberOfPartition, int sizeFile)
{

    rewind(file);

    char partitionName[100];
    char str1[100];
    char str2[100] = ".dat";

    for (int i = 0; i < sizeFile; ++i)
    {

        TSmart *auxSmart = le_smart(file);

        int selectedParticipation = auxSmart->num_fabricacao % numberOfPartition;

        snprintf(str1, sizeof(str1), "%d", selectedParticipation);
        snprintf(partitionName, sizeof(partitionName), "hashTablePartition_smart%s%s", str1, str2);

        FILE *filePartition = fopen(partitionName, "ab+");

        salva_smart(auxSmart, filePartition);

        fclose(filePartition);

    }

    for (int i = 0; i < numberOfPartition; ++i)
    {

        snprintf(str1, sizeof(str1), "%d", i);
        snprintf(partitionName, sizeof(partitionName), "hashTablePartition_smart%s%s", str1, str2);

        FILE *filePartition = fopen(partitionName, "ab+");

        printPartitionSmartphoneID(filePartition, partitionName);

        fclose(filePartition);
    }
}

TSmart *searchHashTable_smart(int numf, int partitionID)
{
    char partitionName[100];
    char str1[100];
    char str2[100] = ".dat";
    int count = 0;

    // Substituir itoa por sprintf para criar o nome da partição
    sprintf(str1, "%d", partitionID);

    strcpy(partitionName, "hashTablePartition_smart");
    strcat(partitionName, str1);
    strcat(partitionName, str2);

    printf("\nSearching in %s...", partitionName);

    FILE *filePartition = fopen(partitionName, "rb+");
    if (filePartition == NULL) {
        printf("\nError opening partition file %s.", partitionName);
        return NULL;
    }

    TSmart *auxSmart = busca_sequencial_smart(filePartition, sizeFile(filePartition, 0), numf, &count);

    fclose(filePartition);

    return auxSmart;
}
void deleteHashTable_smart(int numf, int partitionID) {

    char partitionName[100];
    char str1[100];
    char str2[100] = ".dat";
    int count=0;

    itoa(partitionID, str1, 10);
    strcat(strcpy(partitionName, "hashTablePartition_smart"), str1);
    strcat(strcpy(partitionName, partitionName), str2);

    FILE *filePartition = fopen(partitionName,"r+b");

    TSmart *auxSmart = busca_sequencial_smart(filePartition, sizeFile(filePartition, 0), numf, &count);

    if (auxSmart != NULL) {
        rewind(filePartition);
        fseek(filePartition, (count)*tamanho(), SEEK_SET);

        TSmart smartHashDelet;

        smartHashDelet.num_fabricacao = INT_MAX;
        sprintf(smartHashDelet.nome, "%s", auxSmart->nome);
        sprintf(smartHashDelet.data_fabricacao, "%s", auxSmart->data_fabricacao);
        sprintf(smartHashDelet.num_serie, "%s", auxSmart->num_serie);
        smartHashDelet.valor = auxSmart->valor;

        salva_smart(&smartHashDelet, filePartition);

        printf("\nComputer deleted successfully.");
    } else {
        printf("\nComputer id not exist in the Hash Table.");
    }
    printPartitionSmartphoneID(filePartition, partitionName);
    fclose(filePartition);
}

void insertHashTable_smart(TSmart *smartHashTableInsert, int partitionID) {

    char partitionName[100];
    char str1[100];
    char str2[100] = ".dat";
    int count;
    int countDeleted = 0;

    itoa(partitionID, str1, 10);
    strcat(strcpy(partitionName, "hashTablePartition_smart"), str1);
    strcat(strcpy(partitionName, partitionName), str2);

    FILE *filePartition = fopen(partitionName,"r+b");

    TSmart *auxSmart = busca_sequencial_smart(filePartition, sizeFile(filePartition, 0), smartHashTableInsert->num_fabricacao, &count);
    rewind(filePartition);

    TSmart *auxSmartDeleted = busca_sequencial_smart(filePartition, sizeFile(filePartition, 0), INT_MAX, &countDeleted);
    rewind(filePartition);

    if (auxSmart == NULL && auxSmartDeleted == NULL) {
        fseek(filePartition, (sizeFile(filePartition, 0)) * tamanho(), SEEK_SET);\
        salva_smart(smartHashTableInsert, filePartition);
        printf("\nEmployee entered successfully at the end.");
        printPartitionSmartphoneID(filePartition, partitionName);
    } else if (auxSmart == NULL && auxSmartDeleted != NULL){
        fseek(filePartition, (countDeleted)*tamanho(), SEEK_SET);\
        salva_smart(smartHashTableInsert, filePartition);
        printPartitionSmartphoneID(filePartition, partitionName);
        printf("\nComputer entered successfully at computer previous deleted.");
    } else {
        printf("\nComputer already exist in the Hash Table.");

    }

    fclose(filePartition);
}

