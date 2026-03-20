#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_HILOS 12   // 12 nucleos reales

int *A, *B, *B_T, *C;
int N;

typedef struct {
    int inicio;
    int fin;
} DatosHilo;

/* -----------------------------
   TRANSPONER MATRIZ B
--------------------------------*/
void transponer_B() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            B_T[j*N + i] = B[i*N + j];
        }
    }
}

/* -----------------------------
   MULTIPLICACION SECUENCIAL
--------------------------------*/
void multiplicar_secuencial() {

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {

            int suma = 0;

            for (int k = 0; k < N; k++) {
                suma += A[i*N + k] * B_T[j*N + k];
            }

            C[i*N + j] = suma;
        }
    }
}

/* -----------------------------
   FUNCION DE HILO
--------------------------------*/
void *multiplicar_paralelo(void *arg) {

    DatosHilo *datos = (DatosHilo *)arg;

    for (int i = datos->inicio; i < datos->fin; i++) {
        for (int j = 0; j < N; j++) {

            int suma = 0;

            for (int k = 0; k < N; k++) {
                suma += A[i*N + k] * B_T[j*N + k];
            }

            C[i*N + j] = suma;
        }
    }

    pthread_exit(NULL);
}

/* -----------------------------
   MAIN
--------------------------------*/
int main(int argc, char *argv[]) {

    if (argc < 3) {
        printf("Uso:\n");
        printf("./matriz_transponer <tamano_matriz> <modo>\n");
        printf("modo 0 = secuencial\n");
        printf("modo 1 = paralelo (12 hilos)\n");
        return 1;
    }

    N = atoi(argv[1]);
    int modo = atoi(argv[2]);

    srand(time(NULL));

    /* Reservar memoria contigua */
    A   = (int*) malloc(N * N * sizeof(int));
    B   = (int*) malloc(N * N * sizeof(int));
    B_T = (int*) malloc(N * N * sizeof(int));
    C   = (int*) malloc(N * N * sizeof(int));

    if (!A || !B || !B_T || !C) {
        printf("Error reservando memoria\n");
        return 1;
    }

    /* Inicializar matrices */
    for (int i = 0; i < N*N; i++) {
        A[i] = rand() % 10;
        B[i] = rand() % 10;
        C[i] = 0;
    }

    /* Transponer B */
    transponer_B();

    /* -------- MEDICION CORRECTA (TIEMPO REAL) -------- */
    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    if (modo == 0) {

        printf("Modo SECUENCIAL (cache optimizado)\n");
        multiplicar_secuencial();

    } else {

        printf("Modo PARALELO con %d hilos (12 nucleos)\n", NUM_HILOS);

        pthread_t hilos[NUM_HILOS];
        DatosHilo datos[NUM_HILOS];

        int filas_por_hilo = N / NUM_HILOS;

        for (int i = 0; i < NUM_HILOS; i++) {

            datos[i].inicio = i * filas_por_hilo;

            if (i == NUM_HILOS - 1)
                datos[i].fin = N;
            else
                datos[i].fin = (i + 1) * filas_por_hilo;

            pthread_create(&hilos[i], NULL, multiplicar_paralelo, &datos[i]);
        }

        for (int i = 0; i < NUM_HILOS; i++) {
            pthread_join(hilos[i], NULL);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);

    double tiempo = (fin.tv_sec - inicio.tv_sec) +
                    (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("Tiempo real de ejecucion: %f segundos\n", tiempo);

    /* Guardar resultados */
    FILE *archivo = fopen("resultados_cache.csv", "a");
    if (archivo != NULL) {
        fprintf(archivo, "%d,%d,%f\n", N, modo, tiempo);
        fclose(archivo);
    }

    free(A);
    free(B);
    free(B_T);
    free(C);

    return 0;
}