#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main(int argc, char *argv) {
    if (argc!= 2) {
        fprintf(stderr, "Uso: %s <numero_de_lanzamientos>\n", argv);
        return 1;
    }

    long long total_lanzamientos = atoll(argv);
    if (total_lanzamientos <= 0) {
        fprintf(stderr, "El número de lanzamientos debe ser un entero positivo.\n");
        return 1;
    }

    const double L = 1.0; // Longitud de la aguja
    const double T = 1.0; // Distancia entre líneas

    long long cruces = 0;
    
    // Sembrar el generador de números aleatorios
    srand(time(NULL));

    for (long long i = 0; i < total_lanzamientos; ++i) {
        // Generar una distancia aleatoria del centro de la aguja a la línea más cercana
        double x = ((double)rand() / RAND_MAX) * (T / 2.0);

        // Generar un ángulo aleatorio de la aguja
        double theta = ((double)rand() / RAND_MAX) * M_PI;

        // Comprobar la condición de cruce
        if (x <= (L / 2.0) * sin(theta)) {
            cruces++;
        }
    }

    // Calcular la estimación de PI
    if (cruces > 0) {
        double pi_estimado = (2.0 * L * total_lanzamientos) / (T * cruces);
        printf("Lanzamientos totales: %lld\n", total_lanzamientos);
        printf("Cruces totales: %lld\n", cruces);
        printf("Estimación de PI: %.12f\n", pi_estimado);
    } else {
        printf("No se registraron cruces. No se puede estimar PI.\n");
    }

    return 0;
}