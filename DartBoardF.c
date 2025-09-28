#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <math.h>

int main(int argc, char *argv[]) {
    long k, n, hits;
    int num_procs;
    const double factor = 1.0 / RAND_MAX;
    double pi_approx = 0;

    if (argc < 3) {
        printf("Ingrese los parametros intentos y procesos.\n");
        return 1;
    } else {
        n = atoi(argv[1]);
        num_procs = atoi(argv[2]);
    }

    srand((int)time(NULL));

    clock_t start_time, end_time;
    double cpu_time_used;

    int chunk_size = n / num_procs;
    int remainder = n % num_procs;

    int pipes[num_procs][2];

    // Crear tuberías
    for (int i = 0; i < num_procs; ++i) {
        if (pipe(pipes[i]) == -1) {
            perror("Error al crear la tubería.");
            exit(EXIT_FAILURE);
        }
    }

    pid_t pid;
    start_time = clock();
    for (int i = 0; i < num_procs; ++i) {
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Error al crear el proceso hijo.\n");
            return 1;
        } else if (pid == 0) { // Proceso hijo
            close(pipes[i][0]); // Cerrar extremo de lectura de la tubería
            long local_hits = 0;
            for (k = 0; k < chunk_size + (i < remainder ? 1 : 0); ++k) {
                double y = rand() * factor;
                double x = rand() * factor;
                if (x * x + y * y < 1.0)
                    ++local_hits;
            }
            write(pipes[i][1], &local_hits, sizeof(long)); // Escribir hits en la tubería
            close(pipes[i][1]); // Cerrar extremo de escritura de la tubería
            exit(0);
        } else { // Proceso padre
            close(pipes[i][1]); // Cerrar extremo de escritura de la tubería en el proceso padre
            long local_hits;
            read(pipes[i][0], &local_hits, sizeof(long)); // Leer hits de la tubería
            close(pipes[i][0]); // Cerrar extremo de lectura de la tubería
            hits += local_hits;
        }
    }

    // Esperar a que todos los procesos hijos terminen
    for (int i = 0; i < num_procs; ++i) {
        wait(NULL);
    }

    pi_approx = 4.0 * hits / n;
    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    //printf("Aproximación de pi después de %ld intentos: %f\n", n, pi_approx);
    printf("%f segundos\n", cpu_time_used);

    return 0;
}