#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <mpi.h>

int main(int argc, char *argv[]) {

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2) {
        if (rank == 0) {
            printf("Uso: %s <numTrials>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    int numTrials = atoi(argv[1]);   // Número de lanzamientos

    if (numTrials <= 0) {
        if (rank == 0) {
            printf("El valor de numTrials debe ser mayor que cero.\n");
        }
        MPI_Finalize();
        return 1;
    }

    // Configuración del experimento
    double L = 1.0;  // Longitud de la aguja
    double d = 2.0;  // Espacio entre las líneas

    // Inicialización del generador de números aleatorios
    srand(rank);

    int localHits = 0;  // Contador de cruces de línea

    double start_time, end_time;
    start_time = MPI_Wtime();

    // Realizar el experimento local
    for (int i = 0; i < numTrials; ++i) {
        double angle = ((double)rand() / RAND_MAX) * 3.14159265359;  // Ángulo aleatorio en radianes
        double x = (((double)rand() / RAND_MAX) * d) / 2;  // Posición aleatoria en el eje X

        // Comprobar si la aguja cruza una línea
        if (x <= (L / 2) * sin(angle)) {
            localHits++;
        }
    }

    // Sumar los resultados locales usando MPI_Reduce
    int globalHits;
    MPI_Reduce(&localHits, &globalHits, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();

    // Imprimir el tiempo solo en el proceso con rango 0
    if (rank == 0) {
        // Calcular π basado en los resultados del experimento
        if (globalHits > 0) {
            double estimatedPi = (2 * L * numTrials * size) / (d * globalHits);
            printf("Valor estimado de PI: %f \n", estimatedPi);

            // Guardar el tiempo en un archivo
            FILE *fp;
            fp = fopen("tiempos.txt", "a");
            fprintf(fp, "Tiempo en segundos para %d procesos: %f\n", size, end_time - start_time);
            fclose(fp);
        } else {
            printf("No se cruzaron líneas, no se puede calcular PI.");
        }

        printf("%f Segundos.\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
