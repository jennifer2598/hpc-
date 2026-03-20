#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_HILOS 8

int **A, **B, **BT, **C;
int N;

typedef struct{
    int inicio;
    int fin;
} DatosHilo;

/*----------------------------------
TRANSPONER MATRIZ B
----------------------------------*/
void transponer(){

    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            BT[j][i] = B[i][j];
}

/*----------------------------------
MULTIPLICACION SECUENCIAL
----------------------------------*/
void multiplicar_secuencial(){

    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){

            int suma = 0;

            for(int k=0;k<N;k++)
                suma += A[i][k] * BT[j][k];

            C[i][j] = suma;
        }
    }
}

/*----------------------------------
FUNCION PARA HILOS
----------------------------------*/
void *multiplicar_paralelo(void *arg){

    DatosHilo *d = (DatosHilo*)arg;

    for(int i=d->inicio;i<d->fin;i++){
        for(int j=0;j<N;j++){

            int suma = 0;

            for(int k=0;k<N;k++)
                suma += A[i][k] * BT[j][k];

            C[i][j] = suma;
        }
    }

    return NULL;
}

/*----------------------------------
MAIN
----------------------------------*/
int main(int argc, char *argv[]){

    if(argc < 3){
        printf("Uso:\n");
        printf("./matriz N modo\n");
        printf("modo 0 = secuencial\n");
        printf("modo 1 = paralelo\n");
        return 1;
    }

    N = atoi(argv[1]);
    int modo = atoi(argv[2]);

    srand(time(NULL));

    /* RESERVAR MEMORIA */

    A = malloc(N*sizeof(int*));
    B = malloc(N*sizeof(int*));
    BT = malloc(N*sizeof(int*));
    C = malloc(N*sizeof(int*));

    for(int i=0;i<N;i++){
        A[i] = malloc(N*sizeof(int));
        B[i] = malloc(N*sizeof(int));
        BT[i] = malloc(N*sizeof(int));
        C[i] = malloc(N*sizeof(int));
    }

    /* INICIALIZAR MATRICES */

    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++){
            A[i][j] = rand()%10;
            B[i][j] = rand()%10;
            C[i][j] = 0;
        }

    /* OPTIMIZACION DE CACHE */

    transponer();

    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    if(modo == 0){

        printf("Modo SECUENCIAL\n");
        multiplicar_secuencial();

    }else{

        printf("Modo PARALELO con %d hilos\n", NUM_HILOS);

        pthread_t hilos[NUM_HILOS];
        DatosHilo datos[NUM_HILOS];

        int filas = N / NUM_HILOS;

        for(int i=0;i<NUM_HILOS;i++){

            datos[i].inicio = i * filas;

            if(i == NUM_HILOS-1)
                datos[i].fin = N;
            else
                datos[i].fin = (i+1) * filas;

            pthread_create(&hilos[i], NULL, multiplicar_paralelo, &datos[i]);
        }

        for(int i=0;i<NUM_HILOS;i++)
            pthread_join(hilos[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);

    double tiempo =
        (fin.tv_sec - inicio.tv_sec) +
        (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("Tiempo de ejecucion: %f segundos\n", tiempo);

    FILE *f = fopen("resultados_pthread.csv","a");

    if(f){
        fprintf(f,"%d,%d,%f\n",N,modo,tiempo);
        fclose(f);
    }

    /* LIBERAR MEMORIA */

    for(int i=0;i<N;i++){
        free(A[i]);
        free(B[i]);
        free(BT[i]);
        free(C[i]);
    }

    free(A);
    free(B);
    free(BT);
    free(C);

    return 0;
}
