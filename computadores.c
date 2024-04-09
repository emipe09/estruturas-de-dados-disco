#include "computadores.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>


#define MAX_STR_SIZE	30
#define MIN_STR_SIZE	5



int y;
//cria um computador
TComp *computador(int numf, char *nome, char *nums, char *dataf, int val)
{

    TComp *comp = (TComp*) malloc(sizeof(TComp));
    if (comp) memset(comp, 0, sizeof(TComp));
    comp->num_fabricacao = numf;
    strcpy(comp->nome, nome);
    strcpy(comp->num_serie, nums);
    strcpy(comp->data_fabricacao, dataf);
    comp->valor=val;
    return comp;

}
void imprime(TComp *comp)
{

    printf("\nNúmero de Fabricação: %i", comp->num_fabricacao);
    printf("\nNome do Computador: %s", comp->nome);
    printf("Número de Série: %s", comp->num_serie);
    printf("\nData de Fabricação: %s", comp->data_fabricacao);
    printf("\nValor: %i", comp->valor);
    printf("\n**********************************************\n");

}
// Função para salvar um computador no arquivo
void salva(TComp *comp, FILE *in)
{
    fwrite(&comp->num_fabricacao, sizeof(int), 1, in);
    fwrite(comp->nome, sizeof(char), sizeof(comp->nome), in);
    fwrite(comp->num_serie, sizeof(char), sizeof(comp->num_serie), in);
    fwrite(comp->data_fabricacao, sizeof(char), sizeof(comp->data_fabricacao), in);
    fwrite(&comp->valor, sizeof(int), 1, in);
}

void shuffle(TComp* array[], int size)
{
    srand(time(NULL));
    for (int i = size - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        TComp* comp = array[i];
        array[i] = array[j];
        array[j] = comp;
    }
}

// Função para ler um computador do arquivo
TComp *le(FILE *in)
{
    TComp *comp = (TComp *)malloc(sizeof(TComp));
    if (comp == NULL)
    {
        // Verifica se a alocação de memória falhou
        fprintf(stderr, "Erro ao alocar memória para o computador.\n");
        return NULL;
    }

    if (0 >= fread(&comp->num_fabricacao, sizeof(int), 1, in))
    {
        // Se fread retornar 0 ou erro, libera a memória alocada e retorna NULL
        free(comp);
        return NULL;
    }

    fread(comp->nome, sizeof(char), sizeof(comp->nome), in);
    fread(comp->num_serie, sizeof(char), sizeof(comp->num_serie), in);
    fread(comp->data_fabricacao, sizeof(char), sizeof(comp->data_fabricacao), in);
    fread(&comp->valor, sizeof(int), 1, in);
    return comp;
}
//tamanho em bytes do computador
int tamanho()
{
    return sizeof(int)  //cod
           + sizeof(char) * 100 //nome
           + sizeof(char) * 7 //numero de serie
           + sizeof(char) * 11//data de fabricacao
           +sizeof(int); //valor do computador

}
//cria a base de dados de maneira desordenada, recebe o tamanho dela e também o arquivo onde ela será criada
void CriarBaseDeDadosComputadores(int tam, FILE *in)
{
    y=tam;
    int ordem[tam];
    for(int i = 0; i<tam; i++)
    {
        ordem[i]=i+1;

    }
    shuffle(&ordem, tam);
    TComp *comp = (TComp *)malloc(sizeof(TComp));
    char nome[100], nums[7], dataf[11];
    int val;
    for(int i = 0; i<tam; i++)
    {
//        printf("\nDigite o nome do computador: ");
//        fflush(stdin);
//        fgets(nome, sizeof(nome), stdin);
//
//        printf("\nDigite o número de série do computador(2 letras, 4 números): ");
//        fflush(stdin);
//        fgets(nums, sizeof(nums), stdin);
//
//        printf("\nDigite a data de fabricação(DD/MM/AAAA): ");
//        fflush(stdin);
//        fgets(dataf, sizeof(dataf), stdin);
//
//        printf("\nDigite o valor em reais do computador: ");
//        scanf("%i", &val);
//
//        comp = computador(ordem[i], nome, nums, dataf, val);
        comp = computador(ordem[i], "PC", "AA0000", "20/04/2023", 3000);
        salva(comp, in);
    }
    free(comp);

}
void le_base_comp(FILE *in)
{
    printf("\n\nCOMPUTADORES\n\n");
    rewind(in);
    TComp *c;
    while ((c = le(in)) != NULL)
    {
        imprime(c);
        free(c);
    }
}



//tamanho da base
int tam_comp()
{
    return y;
}
TComp* busca_sequencial_comp(FILE *in, int tam, int numf, int *count)
{
    clock_t inicio;
    inicio= clock();
    rewind(in);
    TComp *comp;
    while ((comp = le(in)) != NULL)
    {
        if(comp->num_fabricacao==numf)
        {
            clock_t end;
            end=clock();
            printf("\nNumero de comparaçoes: %d",(*count)+1);
            printf("\nTempo de Execucao:  %f\n", ((float)(end-inicio))/CLOCKS_PER_SEC);
            return comp;

        }
        (*count)++;
    }
    clock_t end;
    end=clock();
    printf("\nNumero de comparaçoes: %d",(*count)+1);
    printf("\nTempo de Execucao:  %f\n", ((float)(end-inicio))/CLOCKS_PER_SEC);
    return NULL;
}

void insertion_sort_disco_comp(FILE *in, int tam)
{
    clock_t inicio=clock();
    int i, count=0;
    for (int j = 2; j <= tam; j++)
    {
        fseek(in, (j-1) * tamanho(), SEEK_SET);
        TComp *compj = le(in);
        i = j - 1;
        fseek(in, (i-1) * tamanho(), SEEK_SET);
        TComp *compi = le(in);
        while ((i > 0) && (compi->num_fabricacao > compj->num_fabricacao))
        {
            fseek(in, i * tamanho(), SEEK_SET);
            salva(compi, in);
            count++;
            i = i - 1;
            if (i > 0)
            {
                fseek(in, (i-1) * tamanho(), SEEK_SET);
                compi = le(in);
            }
        }
        fseek(in, (i) * tamanho(), SEEK_SET);
        salva(compj, in);
    }
    fflush(in);
    le_base_comp(in);
    clock_t end= clock();
    printf("\nNúmero de comparações: %i", count);
    printf("\nTempo de Execucao:  %f\n", ((float)(end-inicio))/CLOCKS_PER_SEC);
}
void insertion_sort_disco_override(FILE *in)
{
    // Contar quantos registros existem no arquivo
    fseek(in, 0, SEEK_END);
    long tamanho_arquivo = ftell(in);
    int num_registros = tamanho_arquivo / tamanho();
    fseek(in, 0, SEEK_SET);

    // Adaptar a variável 'tam' para 'num_registros'
    int tam = num_registros;

    clock_t inicio = clock();
    int i, count = 0;

    for (int j = 2; j <= tam; j++)
    {
        fseek(in, (j - 1) * tamanho(), SEEK_SET);
        TComp *compj = le(in);
        i = j - 1;
        fseek(in, (i - 1) * tamanho(), SEEK_SET);
        TComp *compi = le(in);

        while ((i > 0) && (compi->num_fabricacao > compj->num_fabricacao))
        {
            fseek(in, i * tamanho(), SEEK_SET);
            salva(compi, in);
            count++;
            i = i - 1;

            if (i > 0)
            {
                fseek(in, (i - 1) * tamanho(), SEEK_SET);
                compi = le(in);
            }
        }

        fseek(in, i * tamanho(), SEEK_SET);
        salva(compj, in);
    }

    fflush(in);

    clock_t end = clock();
    printf("\nNúmero de comparações: %i", count);
    printf("\nTempo de Execução: %f\n", ((float)(end - inicio)) / CLOCKS_PER_SEC);
}


TComp* busca_binaria_comp(int numf, FILE *in, int tam)
{
    clock_t inicio=clock();
    int left = 0, right = tam - 1, comps=0;

    while(left <= right)
    {
        int middle = (left + right) / 2;
        fseek(in, middle * tamanho(), SEEK_SET);
        TComp* comp = le(in);
        if(numf == comp->num_fabricacao)
        {
            clock_t end = clock();
            printf("\nNumero de Comparacoes por Busca Binaria: %d ",comps+1);
            printf("\nTempo de Execucao:  %f\n", ((float)(end-inicio))/CLOCKS_PER_SEC);
            return comp;
        }
        else if(comp->num_fabricacao < numf)
        {
            left = middle + 1;
        }
        else
        {
            right = middle - 1;
        }
        comps++;
    }
    clock_t end= clock();
    printf("Elemento nao encontrado...  Tempo de execuçao:  %f", ((float)(end-inicio))/CLOCKS_PER_SEC);

    return NULL;
}



int sizeFile(FILE *file, int contSizeFile)
{
    fseek(file, 0, SEEK_SET);
    while (!feof(file))
    {
        fseek(file, contSizeFile * tamanho(), SEEK_SET);
        TComp *aux = le(file);
        if (aux != NULL)
        {
            contSizeFile++;
        }
    }

    return contSizeFile;
}

void printPartitionComputerID(FILE *file, char partitionName[])
{
    printf("\nIDS dos computadores por particao %s: \n --->  ", partitionName);

    for (int i = 0; i < sizeFile(file, 0); ++i)
    {
        fseek(file, i * tamanho(), SEEK_SET);
        TComp *aux = le(file);
        printf(" %i ", aux->num_fabricacao);
    }

    printf("\n");
}

int allVetFrozen(int vet[6])
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


int substitutionSelection(FILE *file, char nameFilePartition[])
{
    int numberOfPartition = 0, contSizeFile = 0, position = 6, smallElementPosition = 0, smallElement = 999999, sizeFileAux = 0, selectedPosition = 0;
    TComp comp[6];
    int auxVetComp[6] = {0, 0, 0, 0, 0, 0};

    rewind(file);

    sizeFileAux = sizeFile(file, contSizeFile);

    printf("\nPerforming substitution selection...");

    for (int i = 0; i < 6; ++i)
    {
        fseek(file, i * tamanho(), SEEK_SET);
        TComp *aux = le(file);
        printf("%i\n", aux->num_fabricacao);
        comp[i] = *aux;
    }

    for (int i = 0; i < 6; ++i)
    {
        auxVetComp[i] = comp[i].num_fabricacao;
    }

    while (position < sizeFileAux)
    {
        char partitionName[100];
        char str1[100];
        char str2[100] = ".dat";

        snprintf(str1, 100, "%d", numberOfPartition);

        // Concatena o nome da particao especifco
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
                if (smallElement > auxVetComp[i] && auxVetFrozen[i] != 1)
                {
                    smallElement = auxVetComp[i];
                    smallElementPosition = i;
                }
            }

            salva(&comp[smallElementPosition], filePartition);

            fseek(file, position * tamanho(), SEEK_SET);
            TComp *aux = le(file);

            position++;
            auxVetComp[smallElementPosition] = aux->num_fabricacao;
            comp[smallElementPosition] = *aux;

            if (aux->num_fabricacao < smallElement)
            {
                auxVetFrozen[smallElementPosition] = 1;
            }

            if (allVetFrozen(auxVetFrozen) == 1)
            {
                numberOfPartition++;
                break;
            }
        }

        fclose(filePartition);

        if (position >= sizeFileAux)
        {
            // Cria um vetor pros elementos restantes
            TComp remainingElements[6];
            int numRemaining = 0;

            // Pega todos os elementos restantes que nao estao congelados
            for (int i = 0; i < 6; ++i)
            {
                if (auxVetFrozen[i] == 0)
                {
                    remainingElements[numRemaining] = comp[i];
                    numRemaining++;
                }
            }

            // Ordena os elementos restantes de maneira crescente
            for (int i = 0; i < numRemaining - 1; ++i)
            {
                for (int j = 0; j < numRemaining - i - 1; ++j)
                {
                    if (remainingElements[j].num_fabricacao > remainingElements[j + 1].num_fabricacao)
                    {
                        TComp temp = remainingElements[j];
                        remainingElements[j] = remainingElements[j + 1];
                        remainingElements[j + 1] = temp;
                    }
                }
            }

            // Abre o arquivo da ultima particao
            char partitionNameLast[100];
            char strLast[100];
            char str2Last[100] = ".dat";
            snprintf(strLast, 100, "%d", numberOfPartition);
            strcpy(partitionNameLast, nameFilePartition);
            strcat(partitionNameLast, strLast);
            strcat(partitionNameLast, str2Last);

            FILE *filePartitionLast = fopen(partitionNameLast, "ab+");

            // Escreve os elementos restastantes na ultima particao
            for (int i = 0; i < numRemaining; ++i)
            {
                salva(&remainingElements[i], filePartitionLast);
            }

            // Fecha o arquivo da ultima particao
            fclose(filePartitionLast);

            break;
        }

    }
    //fecha todos os arquivos de particoes e por fim retorna o numero de particoes criadas

    for (int i = 0; i <= numberOfPartition; ++i)
    {
        char partitionName[100];
        char str1[100];
        char str2[100] = ".dat";

        snprintf(str1, 100, "%d", i);
        strcat(strcpy(partitionName, nameFilePartition), str1);
        strcat(partitionName, str2);

        FILE *filePartition = fopen(partitionName, "rb+");
        printPartitionComputerID(filePartition, partitionName);
        fclose(filePartition);
    }
    return numberOfPartition+1;
}

//realiza a intercalação de um array de particoes de tamanho F-1 em um só arquivo
void merge(FILE *inputFiles[], int numInputFiles, FILE *outputFile)
{
    TComp *compArray[numInputFiles];
    int minIndex, i;

    for (i = 0; i < numInputFiles; i++)
    {
        compArray[i] = le(inputFiles[i]);
    }

    while (1)
    {
        minIndex = -1;
        for (i = 0; i < numInputFiles; i++)
        {
            if (compArray[i] != NULL)
            {
                if (minIndex == -1 || compArray[i]->num_fabricacao < compArray[minIndex]->num_fabricacao)
                {
                    minIndex = i;
                }
            }
        }

        if (minIndex == -1)
        {
            break;
        }

        salva(compArray[minIndex], outputFile);
        compArray[minIndex] = le(inputFiles[minIndex]);
    }

    for (i = 0; i < numInputFiles; i++)
    {
        free(compArray[i]);
    }
}

//funcao principal da intercalação ótima, que tem como parametros o prefixo da particao, o número de partições criadas
// e um valor inteiro f que vai controlar as fases da intercalação
void optimalMergeSort(char *partitionPrefix, int numPartitions, int f)
{
    FILE *inputFiles[f - 1];
    char partitionName[100];
    //vai ser usado para saber o numero da ultima particao criada, que vai ser o arquivo final intercalado
    int x=numPartitions;
    //vai ser usado para controlar o numero da particao criada após F-1 fases
    int y=x;
    int numFilesToMerge=0;
    int count=0;
    //loop até que reste apenas uma particao
    while (numPartitions > 1)
    {
        if (numPartitions < f)
        {
            numFilesToMerge = numPartitions;
        }
        else
        {
            numFilesToMerge = f-1;
        }
        //preenche o array com até f-1 particoes
        for (int i = 0; i < numFilesToMerge; i++)
        {
            snprintf(partitionName, sizeof(partitionName), "%s%d.dat", partitionPrefix, i+count*(f-1));
            inputFiles[i] = fopen(partitionName, "rb");
            if (!inputFiles[i])
            {
                perror("Error opening input file");
            }
        }
        //cria a particao de saida da fase
        snprintf(partitionName, sizeof(partitionName), "%s%d.dat", partitionPrefix, y);
        y++;
        FILE *outputFile = fopen(partitionName, "wb");
        if (!outputFile)
        {
            perror("Error opening output file");
            // Você pode adicionar o tratamento de erro apropriado aqui
        }
        //manda para funcao de intercalar array
        merge(inputFiles, numFilesToMerge, outputFile);
        //deleta as particoes ja intercaladas na fase
        for (int i = 0; i < numFilesToMerge; i++)
        {
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
    if (!finalOutputFile)
    {
        perror("Error opening final output file");

    }

    // Imprime a última partição ordenada
    printPartitionComputerID(finalOutputFile, "saida");
    fclose(finalOutputFile);
}

void hashTable(FILE *file, int numberOfPartition, int sizeFile)
{

    rewind(file);

    char partitionName[100];
    char str1[100];
    char str2[100] = ".dat";

    for (int i = 0; i < sizeFile; ++i)
    {

        TComp *auxComp = le(file);

        int selectedPartition = auxComp->num_fabricacao % numberOfPartition;

        snprintf(str1, sizeof(str1), "%d", selectedPartition);
        snprintf(partitionName, sizeof(partitionName), "hashTablePartition%s%s", str1, str2);

        FILE *filePartition = fopen(partitionName, "ab+");

        salva(auxComp, filePartition);

        fclose(filePartition);

    }

    for (int i = 0; i < numberOfPartition; ++i)
    {

        snprintf(str1, sizeof(str1), "%d", i);
        snprintf(partitionName, sizeof(partitionName), "hashTablePartition%s%s", str1, str2);

        FILE *filePartition = fopen(partitionName, "ab+");

        printPartitionComputerID(filePartition, partitionName);

        fclose(filePartition);
    }
}

TComp *searchHashTable(int numf, int partitionID)
{
    char partitionName[100];
    char str1[100];
    char str2[100] = ".dat";
    int count = 0;

    // Substituir itoa por sprintf para criar o nome da partição
    sprintf(str1, "%d", partitionID);

    strcpy(partitionName, "hashTablePartition");
    strcat(partitionName, str1);
    strcat(partitionName, str2);

    printf("\nSearching in %s...", partitionName);

    FILE *filePartition = fopen(partitionName, "rb+");
    if (filePartition == NULL) {
        printf("\nError opening partition file %s.", partitionName);
        return NULL;
    }

    TComp *auxComp = busca_sequencial_comp(filePartition, sizeFile(filePartition, 0), numf, &count);

    fclose(filePartition);

    return auxComp;
}
void deleteHashTable(int numf, int partitionID) {

    char partitionName[100];
    char str1[100];
    char str2[100] = ".dat";
    int count=0;

    itoa(partitionID, str1, 10);
    strcat(strcpy(partitionName, "hashTablePartition"), str1);
    strcat(strcpy(partitionName, partitionName), str2);

    FILE *filePartition = fopen(partitionName,"r+b");

    TComp *auxComp = busca_sequencial_comp(filePartition, sizeFile(filePartition, 0), numf, &count);

    if (auxComp != NULL) {
        rewind(filePartition);
        fseek(filePartition, (count)*tamanho(), SEEK_SET);

        TComp funcHashDelet;

        funcHashDelet.num_fabricacao = INT_MAX;
        sprintf(funcHashDelet.nome, "%s", auxComp->nome);
        sprintf(funcHashDelet.data_fabricacao, "%s", auxComp->data_fabricacao);
        sprintf(funcHashDelet.num_serie, "%s", auxComp->num_serie);
        funcHashDelet.valor = auxComp->valor;

        salva(&funcHashDelet, filePartition);

        printf("\nComputer deleted successfully.");
    } else {
        printf("\nComputer id not exist in the Hash Table.");
    }
    printPartitionComputerID(filePartition, partitionName);
    fclose(filePartition);
}

void insertHashTable(TComp *compHashTableInsert, int partitionID) {

    char partitionName[100];
    char str1[100];
    char str2[100] = ".dat";
    int count;
    int countDeleted = 0;

    itoa(partitionID, str1, 10);
    strcat(strcpy(partitionName, "hashTablePartition"), str1);
    strcat(strcpy(partitionName, partitionName), str2);

    FILE *filePartition = fopen(partitionName,"r+b");

    TComp *auxComp = busca_sequencial_comp(filePartition, sizeFile(filePartition, 0), compHashTableInsert->num_fabricacao, &count);
    rewind(filePartition);

    TComp *auxCompDeleted = busca_sequencial_comp(filePartition, sizeFile(filePartition, 0), INT_MAX, &countDeleted);
    rewind(filePartition);

    if (auxComp == NULL && auxCompDeleted == NULL) {
        fseek(filePartition, (sizeFile(filePartition, 0)) * tamanho(), SEEK_SET);\
        salva(compHashTableInsert, filePartition);
        printf("\nEmployee entered successfully at the end.");
        printPartitionComputerID(filePartition, partitionName);
    } else if (auxComp == NULL && auxCompDeleted != NULL){
        fseek(filePartition, (countDeleted)*tamanho(), SEEK_SET);\
        salva(compHashTableInsert, filePartition);
        printPartitionComputerID(filePartition, partitionName);
        printf("\nComputer entered successfully at computer previous deleted.");
    } else {
        printf("\nComputer already exist in the Hash Table.");

    }

    fclose(filePartition);
}
