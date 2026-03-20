#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <time.h>

#define NUM_PROCESOS 8

int *A, *B, *BT, *C;
int N;

/*----------------------------------
TRANSPONER MATRIZ B
----------------------------------*/
void transponer(){

    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            BT[j*N+i] = B[i*N+j];
}

/*----------------------------------
MULTIPLICACION SECUENCIAL
----------------------------------*/
void multiplicar_secuencial(){

    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){

            int suma = 0;

            for(int k=0;k<N;k++)
                suma += A[i*N+k] * BT[j*N+k];

            C[i*N+j] = suma;
        }
    }
}

/*----------------------------------
MULTIPLICACION PARCIAL (PROCESO)
----------------------------------*/
void multiplicar_parcial(int inicio,int fin){

    for(int i=inicio;i<fin;i++){
        for(int j=0;j<N;j++){

            int suma = 0;

            for(int k=0;k<N;k++)
                suma += A[i*N+k] * BT[j*N+k];

            C[i*N+j] = suma;
        }
    }
}

/*----------------------------------
MAIN
----------------------------------*/
int main(int argc,char *argv[]){

    if(argc < 3){
        printf("Uso:\n");
        printf("./matriz_fork N modo\n");
        printf("modo 0 = secuencial\n");
        printf("modo 1 = paralelo\n");
        return 1;
    }

    N = atoi(argv[1]);
    int modo = atoi(argv[2]);

    srand(time(NULL));

    /* MEMORIA COMPARTIDA */

    A  = mmap(NULL,N*N*sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    B  = mmap(NULL,N*N*sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    BT = mmap(NULL,N*N*sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    C  = mmap(NULL,N*N*sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);

    /* INICIALIZAR MATRICES */

    for(int i=0;i<N*N;i++){
        A[i] = rand()%10;
        B[i] = rand()%10;
        C[i] = 0;
    }

    /* OPTIMIZACION DE CACHE */

    transponer();

    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    if(modo == 0){

        printf("Modo SECUENCIAL (optimizado para cache)\n");
        multiplicar_secuencial();

    }else{

        printf("Modo PARALELO con %d procesos\n", NUM_PROCESOS);

        int filas = N / NUM_PROCESOS;

        for(int p=0;p<NUM_PROCESOS;p++){

            pid_t pid = fork();

            if(pid == 0){

                int ini = p * filas;

                int fin_f;

                if(p == NUM_PROCESOS-1)
                    fin_f = N;
                else
                    fin_f = (p+1) * filas;

                multiplicar_parcial(ini, fin_f);

                exit(0);
            }
        }

        for(int p=0;p<NUM_PROCESOS;p++)
            wait(NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);

    double tiempo =
        (fin.tv_sec - inicio.tv_sec) +
        (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("Tiempo de ejecucion: %f segundos\n", tiempo);

    FILE *f = fopen("resultados_fork.csv","a");

    if(f){
        fprintf(f,"%d,%d,%f\n",N,modo,tiempo);
        fclose(f);
    }

    return 0;
}
