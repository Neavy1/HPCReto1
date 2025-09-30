#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void simular_en_hijo(long long lanzamientos, int pipe_fd) {
    // Resembrar el PRNG en el proceso hijo con una semilla única
    srand(time(NULL) ^ getpid());

    long long cruces_locales = 0;
    const double L = 1.0;
    const double T = 1.0;

    for (long long i = 0; i < lanzamientos; ++i) {
        double x = ((double)rand() / RAND_MAX) * (T / 2.0);
        double theta = ((double)rand() / RAND_MAX) * M_PI;

        if (x <= (L / 2.0) * sin(theta)) {
            cruces_locales++;
        }
    }

    write(pipe_fd, &cruces_locales, sizeof(cruces_locales));
    close(pipe_fd); 
    exit(0);
}

int main(int argc, char *argv) {
    if (argc!= 3) {
        fprintf(stderr, "Uso: %s <numero_de_lanzamientos> <numero_de_procesos>\n", argv);
        return 1;
    }

    long long total_lanzamientos = atoll(argv);
    int num_procesos = atoi(argv);

    if (total_lanzamientos <= 0 |

| num_procesos <= 0) {
        fprintf(stderr, "Los lanzamientos y los procesos deben ser enteros positivos.\n");
        return 1;
    }

    int pipes[num_procesos];
    pid_t pids[num_procesos];
    long long lanzamientos_por_proceso = total_lanzamientos / num_procesos;

    for (int i = 0; i < num_procesos; ++i) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pids[i] = fork();

        if (pids[i] < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0) { 
            close(pipes[i]); 
            long long mis_lanzamientos = (i == 0)? lanzamientos_por_proceso + (total_lanzamientos % num_procesos) : lanzamientos_por_proceso;
            simular_en_hijo(mis_lanzamientos, pipes[i]);
        }
    }

    // Proceso padre
    long long cruces_totales = 0;
    for (int i = 0; i < num_procesos; ++i) {
        close(pipes[i]);
        long long cruces_hijo;
        read(pipes[i], &cruces_hijo, sizeof(cruces_hijo));
        cruces_totales += cruces_hijo;
        close(pipes[i]);
    }

    // Esperar a que todos los hijos terminen
    for (int i = 0; i < num_procesos; ++i) {
        waitpid(pids[i], NULL, 0);
    }

    if (cruces_totales > 0) {
        double pi_estimado = (2.0 * 1.0 * total_lanzamientos) / (1.0 * cruces_totales);
        printf("Lanzamientos totales: %lld\n", total_lanzamientos);
        printf("Procesos: %d\n", num_procesos);
        printf("Cruces totales: %lld\n", cruces_totales);
        printf("Estimación de PI: %.12f\n", pi_estimado);
    } else {
        printf("No se registraron cruces.\n");
    }

    return 0;
}