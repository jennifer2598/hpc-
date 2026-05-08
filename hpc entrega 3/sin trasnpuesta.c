#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int **A, **B, **BT, **C;
int N;

// Reservar memoria
void reservar() {
    A = (int**) malloc(N * sizeof(int*));
    B = (int**) malloc(N * sizeof(int*));
    BT = (int**) malloc(N * sizeof(int*));
    C = (int**) malloc(N * sizeof(int*));

    for(int i = 0; i < N; i++){
        A[i] = (int*) malloc(N * sizeof(int));
        B[i] = (int*) malloc(N * sizeof(int));
        BT[i] = (int*) malloc(N * sizeof(int));
        C[i] = (int*) malloc(N * sizeof(int));
    }
}

// Inicializar matrices
void inicializar() {
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
            C[i][j] = 0;
        }
    }
}

// Transponer matriz B
void transponer() {
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            BT[j][i] = B[i][j];
        }
    }
}

// Multiplicación optimizada (usando BT)
void multiplicar() {
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            int suma = 0;
            for(int k = 0; k < N; k++){
                suma += A[i][k] * B[k][j];  // acceso por columnas ?
            }
            C[i][j] = suma;
        }
    }
}

// Liberar memoria
void liberar() {
    for(int i = 0; i < N; i++){
        free(A[i]);
        free(B[i]);
        free(BT[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(BT);
    free(C);
}

int main(int argc, char *argv[]) {

    if(argc < 2){
        printf("Uso: %s N\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    srand(time(NULL));

    reservar();
    inicializar();
    

    clock_t inicio = clock();

    /*transponer();   */// mejora acceso a memoria
    multiplicar();  // algoritmo principal

    clock_t fin = clock();

    double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;

    printf("Tiempo secuencial con transpuesta: %f segundos\n", tiempo);

    liberar();
    return 0;
}
