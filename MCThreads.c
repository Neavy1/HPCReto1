#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>

using namespace std;

// Estructura para pasar datos a la función del hilo
struct ThreadData {
    int numTrials;
    double L;
    double d;
    int hits;
};

// Función que realiza los lanzamientos en un hilo
void* lanzarAgujas(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    
    for (int i = 0; i < data->numTrials; ++i) {
        double angle = (rand() / (double)RAND_MAX) * 3.14159265359;
        double x = ((rand() / (double)RAND_MAX) * data->d) / 2;

        if (x <= (data->L / 2) * sin(angle)) {
            data->hits++;
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <numTrials> <numThreads>\n", argv[0]);
        return 1;
    }

    int numTrials = atoi(argv[1]);   // Número de lanzamientos
    int numThreads = atoi(argv[2]);

    if (numTrials <= 0 || numThreads<= 0) {
        printf("El valor de numTrials y numThreads debe ser mayor que cero.\n");
        return 1;
    }
    const double L = 1.0;
    const double d = 2.0;

    srand(time(nullptr));
    int totalHits = 0;
    pthread_t threads[numThreads];
    ThreadData threadData[numThreads];

    clock_t start_time,end_time;
    double cpu_time_used;

    start_time = clock();

    for (int i = 0; i < numThreads; ++i) {
        threadData[i].numTrials = numTrials / numThreads;
        threadData[i].L = L;
        threadData[i].d = d;
        threadData[i].hits = 0;
        int result = pthread_create(&threads[i], NULL, lanzarAgujas, &threadData[i]);

        if (result) {
            cerr << "Error al crear el hilo: " << result << endl;
            exit(-1);
        }
    }

    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], NULL);
        totalHits += threadData[i].hits;
    }

    if (totalHits > 0) {
        double estimatedPi = (2 * L * numTrials) / (d * totalHits);
        printf("Valor estimado de PI: %f \n", estimatedPi);
    } else {
        printf("No se cruzaron líneas, no se puede calcular PI.");
    }

    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("%f Segundos.",cpu_time_used);
    pthread_exit(NULL);
}
