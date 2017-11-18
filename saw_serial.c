#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<float.h>

#define lin 4
#define col 4

//Para compilar gcc saw_serial.c -o saw_serial

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
    int i,j;
    for(i=0; i<lin; i++)
    {
        for(j=0; j<col; j++)
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
    int i,j;

    //Multiplicação da Matriz de dados pelo Vetor de pesos colocando em "vetorResul"
    for(i=0; i<lin; i++)
    {
        vetorResul[i] = 0;
        for(j=0; j<col; j++)
            vetorResul[i] += matrizDadosNormalizada[i][j] * vetorPesos[j];
    }
}

void exibivetorMaior(float vetorMaior[col])
{
    int i;
    printf("Maior elemento de cada coluna!\n"); //exibe o vetor com o maior elemento de cada linha
    for(i=0; i<col; i++)
    {
        printf("%.2f\n",vetorMaior[i]);
    }
}

void exibivetorMenor(float vetorMenor[col])
{
    int i;
    printf("Menor elemento de cada coluna!\n"); //exibe o vetor com o maior elemento de cada linha
    for(i=0; i<col; i++)
    {
        printf("%.2f\n",vetorMenor[i]);
    }
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

void exibiMatriz(float matrizDados[lin][col])
{
    //imprime matriz normal
    int i,j;
    printf("\n Matriz normal\n");
    for(i=0; i<lin; i++)
    {
        for(j=0; j<col; j++)
        {
            printf("[%f]", matrizDados[i][j]);
        }
        printf("\n");
        return;
    }
}

void exibiMatrizNormalizada(float matrizDadosNormalizada[lin][col])
{
    //imprime matriz normalizada
    int i,j;
    printf("Matriz normalizada\n");
    for(i=0; i<lin; i++)
    {
        for(j=0; j<col; j++)
        {
            printf("[%.3f]", matrizDadosNormalizada[i][j]);
        }
        printf("\n");
    }
}



int main (void )
{
    float matrizDados[lin][col] = { {1,2,3,4},
                                        {5,6,7,8},
                                        {9,10,11,12},
                                        {13,14,15,16}};

    //Declaracao da Matriz e vetores
    float vetorPesos[col]     =  {1, 2, 3, 4};

    float vetorMaior[col] = {};
    float vetorMenor[col] = {};
    float vetorResul[col] = {};

    float matrizDadosNormalizada[lin][col] = {};
    //exibiMatriz(matrizDados);
    CalculavetorMaior(vetorMaior, matrizDados);
    CalculavetorMenor(vetorMenor, matrizDados);
    //exibivetorMaior(vetorMaior);
    //exibivetorMenor(vetorMenor);
    //vetormaiormenor(matrizDados, vetorMaior, vetorMenor, aux1);
    normaliza(matrizDadosNormalizada, matrizDados, vetorMaior, vetorMenor, 1);
    geraRanking(matrizDadosNormalizada,vetorResul,vetorPesos);

    //exibiMatriz(matrizDados);
    //exibiMatrizNormalizada(matrizDadosNormalizada);
    //exibivetorMaior(vetorMaior);
    //exibivetorMenor(vetorMenor);

    //ordena o vetor passando a função ordena pala o qsort
    //qsort(vetorResul,lin,sizeof(float), ordena);
    exibivetorResul(vetorResul);

    return(0);
}
