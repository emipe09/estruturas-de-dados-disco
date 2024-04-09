#include <stdio.h>
#include <stdlib.h>
#include "computadores.h"
#include "smartphones.h"
#include <time.h>



int main(int argc, char** argv)
{
    FILE *out, *out2;
    printf("----------------------MP ENTERPRISE----------------------\n");
    if ((out = fopen("computadores.dat", "wb+")) == NULL || (out2 = fopen("smartphones.dat", "wb+")) == NULL)
    {
        printf("Erro ao abrir um dos arquivos\n");
        exit(1);
    }

    else
    {
        int i, tam, x, z, numf, num, num2;
        do
        {
            printf("\n----------------------MENU------------------------------\n");
            printf("\n- (0) SAIR\n- (1) CRIAR/ADICIONAR BASE DE DADOS P/ OS COMPUTADORES\n- (2) CRIAR/ADICIONAR BASE DE DADOS P/ OS SMARTPHONES");
            printf("\n- (3) BUSCA SEQUENCIAL\n- (4) ORDENAÇÃO EM DISCO \n- (5) BUSCA BINÁRIA (BASE JÁ ORDENADA)\n- (6) IMPRIMIR DETERMINADA BASE DE DADOS");
            printf("\n- (7) CLASSIFICAÇÃO EXTERNA\n- (8) CRIAR TABELA HASH\n- (9) FUNCOES TABELA HASH\n");
            scanf("%i", &i);
            if(i==1)
            {
                printf("\nQual o numero de computadores a ser adicionado?\n");
                scanf("%i", &tam);
                CriarBaseDeDadosComputadores(tam, out);
            }
            else if(i==2)
            {
                printf("\nQual o número de smartphones a ser adicionado?\n");
                scanf("%i", &tam);
                CriarBaseDeDadosSmartphones(tam, out2);
            }
            else if(i==3)
            {
                printf("\nDeseja realizar a busca sequencial em qual base de dados? (1-COMPUTADOR, 2-SMARTPHONES)\n");
                scanf("%i", &x);

                printf("Digite o número de fabricação:\n ");
                scanf("%i", &numf);
                if(x==1)
                {
                    busca_sequencial_comp(out,tam_comp(), numf, 0);
                }
                else if(x==2)
                {
                    busca_sequencial_smart(out2, tam_smart(), numf, 0);
                }

            }
            else if(i==4)
            {
                printf("\nDeseja realizar o Insertion Sort em 1-Base dos Computadores, 2-Base dos Smartphones?\n");
                scanf("%i", &x);
                if(x==1)
                {
                    insertion_sort_disco_comp(out, tam_comp());
                }
                else if(x==2)
                {
                    insertion_sort_disco_smart(out2, tam_smart());
                }

            }
            else if(i==5)
            {
                printf("\nDeseja realizar a busca binária em qual base (1-Computadores, 2-Smartphones)?\n");
                scanf("%i", &x);
                printf("\nDigite o número de fabricação: ");
                scanf("%i", &numf);
                if(x==1)
                {
                    TComp* comp;
                    comp = busca_binaria_comp(numf, out, tam_comp());
                    printf("\nCOMPUTADOR ENCONTRADO: ");
                    imprime(comp);
                }
                if(x==2)
                {
                    TSmart* smart;
                    smart = busca_binaria_smart(numf, out2, tam_smart());
                    printf("\nSMARTPHONE ENCONTRADO: ");
                    imprime(smart);

                }
            }
            else if(i==6)
            {
                printf("\nQuer imprimir a base de dados de 1-Computadores ou 2-Smartphones?\n");
                scanf("%i", &x);
                if(x==1)
                    le_base_comp(out);
                else if(x==2)
                    le_base_smart(out2);
            }
            else if(i==7)
            {
                printf("\nDeseja fazer seleção por substituição em qual base? (1-Computadores, 2-Smartphones)");
                scanf("%i", &x);

                if(x==1)
                {
                    clock_t inicio=clock();
                    char nameFilePartition[] = "partition"; // Nome base das partições (sem extensão)
                    int numberOfPartitions = substitutionSelection(out, nameFilePartition);
                    printf("Número de partições criadas: %d\n", numberOfPartitions);
                    clock_t end;
                    end=clock();
                    printf("\nTempo de Execucao:  %f\n", ((float)(end-inicio))/CLOCKS_PER_SEC);
                    printf("Prosseguir para a intercalação? (1-S, 2-N)\n");
                    scanf("%i", &z);
                    if(z==1)
                    {
                        optimalMergeSort("partition", numberOfPartitions, 4);
                    }
                }
                else if(x==2)
                {
                    clock_t inicio=clock();
                    char nameFilePartition[] = "partition_smart"; // Nome base das partições (sem extensão)
                    int numberOfPartitions = substitutionSelection_smart(out2, nameFilePartition);
                    printf("Número de partições criadas: %d\n", numberOfPartitions);
                    clock_t end;
                    end=clock();
                    printf("\nTempo de Execucao:  %f\n", ((float)(end-inicio))/CLOCKS_PER_SEC);
                    printf("Prosseguir para a intercalação? (1-S, 2-N)\n");
                    scanf("%i", &z);
                    if(z==1)
                    {
                        optimalMergeSort_smart("partition_smart", numberOfPartitions, 4);
                    }
                }
            }
            if(i==8)
            {
                printf("Deseja criar a hash table para computadores (1) ou smartphones (2)?\n ");
                scanf("%i", &x);
                if(x==1)
                {
                    printf("Qual o número de partições desejadas na tabela hash?\n");
                    scanf("%i", &num);
                    hashTable(out, num, sizeFile(out, 0));
                    printf("A tabela hash foi criada.\n");
                }
                else if(x==2)
                {
                    printf("Qual o número de partições desejadas na tabela hash?\n");
                    scanf("%i", &num2);
                    hashTable_smart(out2, num2, sizeFile_smart(out2, 0));
                    printf("A tabela hash foi criada.\n");
                }
            }
            if(i==9)
            {
                printf("Deseja mexer na hash table para computadores (1) ou smartphones (2)?\n ");
                scanf("%i", &x);
                if(x==1)
                {
                    printf("O que deseja fazer na tabela hash?\n1-Buscar elemento, 2-Deletar elemento, 3-Inserir elemento");
                    scanf("%i", &z);
                    if(z==1)
                    {
                        int partitionID;

                        printf("\nDigite o numero de fabricação do computador: ");
                        scanf("%i", &numf);

                        partitionID = numf % num;

                        TComp *comp = searchHashTable(numf, partitionID);

                        if (comp != NULL)
                        {
                            imprime(comp);
                        }
                        else
                        {
                            printf("\nComputador não foi encontrado.");
                        }
                    }
                    if(z==2)
                    {
                        int partitionID;

                        printf("\nDigite o numero de fabricação do computador: ");
                        scanf("%i", &numf);

                        partitionID = numf % num;

                        deleteHashTable(numf, partitionID);
                    }
                    if(z==3)
                    {
                        int partitionID;
                        printf("Digite o numero de fabricacao do computador: \n");
                        scanf("%i", &numf);
                        TComp *compHashTableInsert = computador(numf, "Acer Aspire 5", "BB0000", "19-08-2023", 6000);
                        partitionID = compHashTableInsert->num_fabricacao % num;
                        insertHashTable(compHashTableInsert, partitionID);

                    }
                }
                else if(x==2)
                {
                    printf("O que deseja fazer na tabela hash?\n1-Buscar elemento, 2-Deletar elemento, 3-Inserir elemento");
                    scanf("%i", &z);
                    if(z==1)
                    {
                        int partitionID;

                        printf("\nDigite o numero de fabricação do smartphone: ");
                        scanf("%i", &numf);

                        partitionID = numf % num2;

                        TSmart *smart = searchHashTable_smart(numf, partitionID);

                        if (smart != NULL)
                        {
                            imprime_smart(smart);
                        }
                        else
                        {
                            printf("\nSmartphone não foi encontrado.");
                        }
                    }
                    if(z==2)
                    {
                        int partitionID;

                        printf("\nDigite o numero de fabricação do smartphone: ");
                        scanf("%i", &numf);

                        partitionID = numf % num2;

                        deleteHashTable_smart(numf, partitionID);
                    }
                    if(z==3)
                    {
                        int partitionID;
                        printf("Digite o numero de fabricacao do smartphone: \n");
                        scanf("%i", &numf);
                        TSmart *smartHashTableInsert = smartphone(numf, "Acer Aspire 5", "BB0000", "19-08-2023", 6000);
                        partitionID = smartHashTableInsert->num_fabricacao % num2;
                        insertHashTable_smart(smartHashTableInsert, partitionID);

                    }
                }
            }



        }
        while(i!=0);

    }

    return 0;
}

