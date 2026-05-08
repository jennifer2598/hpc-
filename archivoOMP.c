#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int *A, *B, *BT, *C;
int N;

/* TRANSPONER */
void transponer() {
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            BT[j*N + i] = B[i*N + j];
}

/* MULTIPLICACIÓN CON OPENMP */
void multiplicar() {

    #pragma omp parallel for collapse(2)
    for(int i = 0; i < N; i++){
    	
    	/*#pragma omp parallel for*/
        for(int j = 0; j < N; j++){

            int suma = 0;
			#pragma omp parallel for
            for(int k = 0; k < N; k++){
                suma += A[i*N + k] * BT[j*N + k];
            }

            C[i*N + j] = suma;
        }
    }
}

int main(int argc, char *argv[]) {

    if(argc < 2){
        printf("Uso: %s N\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);

    A  = malloc(N*N*sizeof(int));
    B  = malloc(N*N*sizeof(int));
    BT = malloc(N*N*sizeof(int));
    C  = malloc(N*N*sizeof(int));

    for(int i = 0; i < N*N; i++){
        A[i] = rand()%10;
        B[i] = rand()%10;
        C[i] = 0;
    }

    transponer();

    double inicio = omp_get_wtime();

    multiplicar();

    double fin = omp_get_wtime();

    printf("Tiempo OpenMP: %f segundos\n", fin - inicio);

    free(A); free(B); free(BT); free(C);

    return 0;
}
