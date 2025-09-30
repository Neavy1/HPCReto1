#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Uso: %s <numTrials>\n", argv[0]);
        return 1;
    }

    int numTrials = atoi(argv[1]);   // Número de lanzamientos

    if (numTrials <= 0) {
        printf("El valor de numTrials debe ser mayor que cero.\n");
        return 1;
    }


    // Configuración del experimento
    double L = 1.0;  // Longitud de la aguja
    double d = 2.0;  // Espacio entre las líneas

    // Inicialización del generador de números aleatorios
    srand(time(NULL));

    int hits = 0;  // Contador de cruces de línea


	clock_t start_time,end_time;
	double cpu_time_used;
    start_time = clock();
    // Realizar el experimento
    for (int i = 0; i < numTrials; ++i) {
        double angle = (rand() / (double)RAND_MAX) * 3.14159265359;  // Ángulo aleatorio en radianes
        double x = ((rand() / (double)RAND_MAX) * d) / 2;  // Posición aleatoria en el eje X

        // Comprobar si la aguja cruza una línea
        if (x <= (L / 2) * sin(angle)) {
            hits++;
        }
    }

    // Calcular π basado en los resultados del experimento
    if (hits > 0) {
        double estimatedPi = (2 * L * numTrials) / (d * hits);
        printf("Valor estimado de PI: %f \n", estimatedPi);
    } else {
        printf("No se cruzaron líneas, no se puede calcular PI.");
    }
	end_time= clock();
	cpu_time_used = ((double) (end_time-start_time))/CLOCKS_PER_SEC	;
    printf("%f Segundos.",cpu_time_used);
    return 0;
}

