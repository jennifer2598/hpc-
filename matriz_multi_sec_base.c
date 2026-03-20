#include <stdio.h>
#include <stdlib.h>
#include <time.h>

plt.yscale('log')

int **A, **B, **C;
int N;

/* -----------------------------
   MULTIPLICACION SECUENCIAL BASICA
--------------------------------*/
void multiplicar_secuencial() {

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {

            int suma = 0;

            for (int k = 0; k < N; k++) {
                suma += A[i][k] * B[k][j];
            }

            C[i][j] = suma;
        }
    }
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Uso: ./matriz_multi_sec_base <tamano_matriz>\n");
        return 1;
    }

    N = atoi(argv[1]);

    srand(time(NULL));

    /* Reservar memoria */

    A = malloc(N * sizeof(int *));
    B = malloc(N * sizeof(int *));
    C = malloc(N * sizeof(int *));

    for (int i = 0; i < N; i++) {
        A[i] = malloc(N * sizeof(int));
        B[i] = malloc(N * sizeof(int));
        C[i] = malloc(N * sizeof(int));
    }

    /* Llenar matrices */

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }

    /* Medir tiempo */

    clock_t inicio = clock();

    multiplicar_secuencial();

    clock_t fin = clock();

    double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;

    printf("Multiplicacion SECUENCIAL BASICA\n");
    printf("Tiempo de ejecucion: %f segundos\n", tiempo);

    /* Guardar resultados en CSV */

    FILE *archivo = fopen("resultados_cache.csv", "a");

    if (archivo != NULL) {
        fprintf(archivo, "%d,0,%f\n", N, tiempo);
        fclose(archivo);
    }

    /* Liberar memoria */

    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }

    free(A);
    free(B);
    free(C);

    return 0;
}
