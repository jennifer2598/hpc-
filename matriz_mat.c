#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {

    if(argc != 2){
        printf("Uso: %s tamaño_matriz\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);

    int i, j;

    // Reservar memoria para matrices
    double **A = (double**)malloc(N * sizeof(double*));
    double **B = (double**)malloc(N * sizeof(double*));
    double **C = (double**)malloc(N * sizeof(double*));

    for(i = 0; i < N; i++){
        A[i] = (double*)malloc(N * sizeof(double));
        B[i] = (double*)malloc(N * sizeof(double));
        C[i] = (double*)malloc(N * sizeof(double));
    }

    // Inicializar matrices con valores aleatorios
    srand(time(NULL));

    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
        }
    }

    clock_t inicio, fin;

    inicio = clock();

    // SUMA DE MATRICES (algoritmo secuencial)
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            C[i][j] = A[i][j] + B[i][j];
        }
    }

    fin = clock();

    double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;

    printf("%d,%f\n", N, tiempo);

    // Liberar memoria
    for(i = 0; i < N; i++){
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }

    free(A);
    free(B);
    free(C);

    return 0;
}