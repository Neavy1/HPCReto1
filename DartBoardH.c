#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

long n, hits = 0;
const double factor = 1.0 / RAND_MAX;
int NUM_THREADS;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *DartBoard(void *thread_id) {
    long tid = (long)thread_id;
    long chunk_size = n / NUM_THREADS;
    long start = tid * chunk_size;
    long end = start + chunk_size;
    long local_hits = 0;

    for (long k = start; k < end; ++k) {
        double y = rand() * factor;
        double x = rand() * factor;
        if (x * x + y * y < 1.0)
            ++local_hits;
    }

    // Proteger la sección crítica con mutex
    pthread_mutex_lock(&mutex);
    hits += local_hits;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("faltan argumentos.\n");
        return 1;
    }

    n = atoi(argv[1]);
    NUM_THREADS=atoi(argv[2]);

    srand((int)time(NULL));

    pthread_t threads[NUM_THREADS];
    long t;
    int rc;

    clock_t start_time, end_time;
    double cpu_time_used;

    start_time = clock();

    for (t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, DartBoard, (void *)t);
        if (rc) {
            printf("Error al crear el hilo %ld\n", t);
            return 1;
        }
    }

    // Esperar a que todos los hilos terminen
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    double pi_approx = 4.0 * hits / n;

    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    //printf("Aproximación de pi después de %ld intentos: %lf\n", n, pi_approx);
    printf("%f segundos\n", cpu_time_used);

    pthread_exit(NULL);
}