#include <stdio.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#define NUM_THREADS 6 //Cantidad de filas, un thread por fila
#define MAX 10
#define plate_length 6
char matriz_principal [plate_length][plate_length] = {
            {'C', 'C', 'C', 'C', 'C', 'C'},
            {'C', 'C', 'C', 'C', 'C', 'C'},
            {'C', 'C', 'H', 'H', 'H', 'C'},
            {'C', 'C', 'H', 'H', 'H', 'C'},
            {'C', 'C', 'H', 'H', 'H', 'C'},
            {'C', 'C', 'C', 'C', 'C', 'C'},
        };

//Calculo de heat ecuation
void calculate(int matu[10][6][6]){
  int i, j, k;
  float alpha = 2;
  float delta_x = 1;
  float delta_t = (delta_x*delta_x)/(4*alpha);
  float gamma = (alpha*delta_t)/(delta_x*delta_x);
  for (k = 0; k<MAX-1; k++){
    #pragma omp parallel for private(i, j) shared (matu)
    for (i = 0; i<plate_length;i++){
      for (j = 0; j<plate_length;j++){
          if(i == 0 || j == 0 || i == plate_length - 1 || j == plate_length - 1){
            matu[k+1][i][j] = matu[k][i][j];
          }else{
            matu[k+1][i][j] = gamma*(matu[k][i+1][j]+matu[k][i-1][j]+matu[k][i][j+1]+matu[k][i][j-1]-4*matu[k][i][j])+matu[k][i][j];
      
          }
      }
    }
  }  
}



void llenarMatriz(int filas, int columnas, int matriz[filas][columnas]){
  for(int i = 0; i < filas; i++){
    for(int j = 0; j < columnas; j++){
      matriz[i][j] = 0;
      }
  }
}

void* llena_matriz_thread(int matriz_threads[6][6]){ 
    char cold = 'C';
    char hot = 'H';
    for (int y = 0; y < plate_length; y++) {
        for (int x = 0; x < plate_length; x++) {
            if (matriz_principal[y][x] == cold){
                matriz_threads[y][x] = rand() % 61; //Temperatura entre 0 a 60
            }
            else if (matriz_principal[y][x] == hot){
                matriz_threads[y][x] = rand() % 41 + 60; //Temperatura entre 60 a 100
            }
        }    }
}

void imprimirMatriz(int filas, int columnas,char matu[filas][columnas]){
    for(int i = 0; i < filas; i++){
        for(int j = 0; j < columnas; j++){
            printf("%c ",matu[i][j]);
        }
        printf("\n");
    }
}

void imprime_matriz_thread(int matriz_threads[6][6]){ //Que imprima bien la matriz con los caracteres
    for (int x = 0; x<plate_length; x++){
        for (int i = 0; i<plate_length; i++){
            printf("%d ", matriz_threads[x][i]);
            //printf("\n");
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) 
{
        pthread_t threads[NUM_THREADS];
        int rc;
        long t;

        int matriz_threads[MAX][plate_length][plate_length];
        llenarMatriz(plate_length,plate_length,matriz_threads[0]);

        printf("Matriz de temperaturas de caracteres: \n");
        imprimirMatriz(plate_length, plate_length,matriz_principal);
        printf("\n");

        printf("Matriz de temperatura numerica antes del llenado: \n");
        imprime_matriz_thread(matriz_threads[0]);
        printf("\n");

        //THREAD LLENADO
        pthread_t llenado_matriz;
        rc = pthread_create(&llenado_matriz,NULL,&llena_matriz_thread,matriz_threads[0]);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
        pthread_join(llenado_matriz, NULL);

        printf("Matriz de temperatura numerica despues del llenado: \n");
        imprime_matriz_thread(matriz_threads[0]);
        
        //HEAT EQUATION 

        //PROGRAMACION PARALELA
        int thread_num = 1;
        if (argc > 1) {
            thread_num = atoi(argv[1]);
        }
        omp_set_num_threads(thread_num);
        printf("OMP defined, threadct = %d \n", thread_num);

        calculate(matriz_threads);
        printf("Calculo de heat ecuation en matriz: \n");
        imprime_matriz_thread(matriz_threads[9]);
}
