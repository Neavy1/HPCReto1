#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Estructura para pasar argumentos a cada hilo
typedef struct {
    long long lanzamientos_por_hilo;
    unsigned int semilla;
    long long cruces_locales;
} ThreadData;

// Función que será ejecutada por cada hilo
void* simular_lanzamientos(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    data->cruces_locales = 0;

    const double L = 1.0;
    const double T = 1.0;

    for (long long i = 0; i < data->lanzamientos_por_hilo; ++i) {
        // Usar rand_r para generación de números aleatorios segura para hilos
        double x = ((double)rand_r(&data->semilla) / RAND_MAX) * (T / 2.0);
        double theta = ((double)rand_r(&data->semilla) / RAND_MAX) * M_PI;

        if (x <= (L / 2.0) * sin(theta)) {
            data->cruces_locales++;
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv) {
    if (argc!= 3) {
        fprintf(stderr, "Uso: %s <numero_de_lanzamientos> <numero_de_hilos>\n", argv);
        return 1;
    }

    long long total_lanzamientos = atoll(argv);
    int num_hilos = atoi(argv);

    if (total_lanzamientos <= 0 || num_hilos <= 0) {
        fprintf(stderr, "Los lanzamientos y los hilos deben ser enteros positivos.\n");
        return 1;
    }

    pthread_t hilos[num_hilos];
    ThreadData datos_hilos[num_hilos];
    long long lanzamientos_por_hilo = total_lanzamientos / num_hilos;

    // Crear y lanzar los hilos
    for (int i = 0; i < num_hilos; ++i) {
        datos_hilos[i].lanzamientos_por_hilo = (i == 0)? lanzamientos_por_hilo + (total_lanzamientos % num_hilos) : lanzamientos_por_hilo;
        datos_hilos[i].semilla = time(NULL) ^ (i + 1);
        
        int rc = pthread_create(&hilos[i], NULL, simular_lanzamientos, &datos_hilos[i]);
        if (rc) {
            fprintf(stderr, "ERROR; pthread_create() devolvió el código %d\n", rc);
            exit(-1);
        }
    }

    long long cruces_totales = 0;
    // Esperar a que los hilos terminen y agregar sus resultados
    for (int i = 0; i < num_hilos; ++i) {
        pthread_join(hilos[i], NULL);
        cruces_totales += datos_hilos[i].cruces_locales;
    }

    // Calcular la estimación de PI
    if (cruces_totales > 0) {
        double pi_estimado = (2.0 * 1.0 * total_lanzamientos) / (1.0 * cruces_totales);
        printf("Lanzamientos totales: %lld\n", total_lanzamientos);
        printf("Hilos: %d\n", num_hilos);
        printf("Cruces totales: %lld\n", cruces_totales);
        printf("Estimación de PI: %.12f\n", pi_estimado);
    } else {
        printf("No se registraron cruces.\n");
    }

    return 0;
}