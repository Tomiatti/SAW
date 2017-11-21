// para compilar: mpicc saw_mpi.c -o saw_mpi -Wall
// para rodar: mpirun -np 2 saw_mpi

#include <mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<float.h>
#define SIZE 8

#define lin 4
#define col 4

/*vetor de pesos que todos deverão ter acesso*/
float vetorPesos[col] = {1, 2, 3, 4};

//função do qsort para ordenação
int ordena(const void * a,const void * b)
{
    if(*(int*)a == *(int*)b)
        return 0; //iguais
    else if (*(int*)a > *(int*)b)
        return -1; // vem antes
    else
        return 1; // vem depois
}
    
void exibivetorResul(float vetorResul[lin])
{
    int i;
    printf("Impress�o do produto Matriz x Vetor de Pesos\n");
    for(i=0; i<lin; i++)
    {
        printf("Elemento[%d] = %.3f \n", i, vetorResul[i]);
    }
}

void CalculavetorMaior(float vetorMaior[col] , float  matrizDados[lin][col])
{
    for(int j=0; j<col; j++)
    {
        float maior = 0;
        for(int i=0; i<lin; i++)
        {
            if(matrizDados[i][j]>maior)   //este trecho recebe o maior valor da linha da matriz
            {
                maior=matrizDados[i][j];  //e armazena este valor em um vetor
            }
        }
        //printf("%f\n", maior);
        vetorMaior[j]=maior;
    }
}

void CalculavetorMenor(float vetorMenor[col] , float  matrizDados[lin][col])
{
 // int i,j;
    for(int j=0; j<col; j++)
    {
        float menor = FLT_MAX;
        for(int i=0; i<lin; i++)
        {
            if(menor > matrizDados[i][j])
            {
                menor=matrizDados[i][j];
                //printf("%f\n", menor);
            }
        }
        //printf("%lf", menor);
        vetorMenor[j]=menor;
    }
}

//função para normalizar a matriz de dados onde op=0 -> max e op=1 -> min
void normaliza(float matrizDadosNormalizada[lin][col], float matrizDados[lin][col], float vetorMaior[col], float vetorMenor[col], int op)
{
    //int i,j;
    for(int i=0; i<lin; i++)
    {
        for(int j=0; j<col; j++)
        {
            if(op == 0)
            {
                matrizDadosNormalizada[i][j] = ((matrizDados[i][j] - vetorMenor[j]) / (vetorMaior[j] - vetorMenor[j]));
            }
            else if(op == 1)
            {
                matrizDadosNormalizada[i][j] = ((vetorMaior[j] - matrizDados[i][j]) / (vetorMaior[j] - vetorMenor[j]));
            }
        }
    }
}
void geraRanking(float matrizDadosNormalizada[lin][col], float vetorResul[col], float vetorPesos[col])
{
   // int i,j;

    //Multiplicação da Matriz de dados pelo Vetor de pesos colocando em "vetorResul"
    for(int i=0; i<lin; i++)
    {
        vetorResul[i] = 0;
        for(int j=0; j<col; j++)
            vetorResul[i] += matrizDadosNormalizada[i][j] * vetorPesos[j];
    }
}


int main(int argc, char** argv) {
   
        int world_rank;
        int world_size;
        MPI_Status status;
        const int root=0;

        int i;

       float matrizDados[lin][col] = { {1,2,3,4},
                                        {5,6,7,8},
                                        {9,10,11,12},
                                        {13,14,15,16}};
                                        
        
        float vetorMaior[col];
        float vetorMenor[col];
        float vetorResul[col];

        float matrizDadosNormalizada[lin][col];

        MPI_Init(&argc, &argv);
        //MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);

        if(world_rank == root) {
            CalculavetorMaior(vetorMaior, matrizDados);
            CalculavetorMenor(vetorMenor, matrizDados);          
        }

        /* apenas para debugar
        for(int i = 0; i< col; i++){
             printf("[%d]: Before Bcast, vetormaior[%d] is [%f] \n", world_rank ,i , vetorMaior[i]);
             printf("[%d]: Before Bcast, vetormenor[%d] is [%f] \n", world_rank ,i , vetorMenor[i]);
        }*/
            
        /* broadcast pra todo mundo sobre os valores de vetorMaior e VetorMenor*/
        MPI_Bcast(vetorMaior, col, MPI_INT, root, MPI_COMM_WORLD);
        MPI_Bcast(vetorMenor, col, MPI_INT, root, MPI_COMM_WORLD);
        /* apenas para debugar
        for(int i = 0; i< col; i++){
            printf("[%d]: After Bcast, vetormaior[%d] is [%f] \n", world_rank ,i , vetorMaior[i]);
            printf("[%d]: After Bcast, vetormenor[%d] is [%f] \n", world_rank ,i , vetorMenor[i]);
        }*/


        /*valores por processo*/
       // int tamanhomatriz = lin*col/world_size;

        MPI_Scatter(matrizDados, lin*col/world_size, MPI_FLOAT, &vetorResul, lin*col/world_size, MPI_FLOAT, 0, MPI_COMM_WORLD);
       
        normaliza(matrizDadosNormalizada, matrizDados, vetorMaior, vetorMenor, 0);
        geraRanking(matrizDadosNormalizada,vetorResul,vetorPesos);

        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Gather(&vetorResul, lin, MPI_FLOAT, matrizDados, lin, MPI_FLOAT, 0, MPI_COMM_WORLD);
        
        qsort(vetorResul,lin,sizeof(float), ordena);
      
    MPI_Finalize(); 

         exibivetorResul(vetorResul);

    return 0;
}
