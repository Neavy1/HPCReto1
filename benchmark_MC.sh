#!/bin/bash

# --- Configuración ---
LANZAMIENTOS=50000000
REPETICIONES=5
TRABAJADORES=(1 2 4 8 12 16)
ARCHIVO_SALIDA="resultados.csv"

# --- Compilación ---
echo "Compilando programas..."
gcc buffon_serial.c -o buffon_serial -lm |

| { echo "Fallo en la compilación de serial"; exit 1; }
gcc buffon_pthreads.c -o buffon_pthreads -lm -lpthread |

| { echo "Fallo en la compilación de pthreads"; exit 1; }
gcc buffon_fork.c -o buffon_fork -lm |

| { echo "Fallo en la compilación de fork"; exit 1; }
echo "Compilación completada."

# --- Encabezado del CSV ---
echo "Implementacion,Trabajadores,Repeticion,Tiempo" > $ARCHIVO_SALIDA

# --- Bucle de Pruebas ---

# Prueba Serial (se ejecuta con 1 trabajador como referencia)
echo "Ejecutando prueba Serial..."
for i in $(seq 1 $REPETICIONES); do
    echo "  Repetición $i/$REPETICIONES"
    # Capturar solo la última línea de la salida (el tiempo)
    tiempo=$(./buffon_serial $LANZAMIENTOS | tail -n 1 | cut -d' ' -f3)
    echo "Serial,1,$i,$tiempo" >> $ARCHIVO_SALIDA
done

# Prueba Pthreads
echo "Ejecutando prueba Pthreads..."
for w in "${TRABAJADORES[@]}"; do
    echo "  Con $w hilos..."
    for i in $(seq 1 $REPETICIONES); do
        echo "    Repetición $i/$REPETICIONES"
        tiempo=$(./buffon_pthreads $LANZAMIENTOS $w | tail -n 1 | cut -d' ' -f3)
        echo "Pthreads,$w,$i,$tiempo" >> $ARCHIVO_SALIDA
    done
done

# Prueba Fork
echo "Ejecutando prueba Fork..."
for w in "${TRABAJADORES[@]}"; do
    echo "  Con $w procesos..."
    for i in $(seq 1 $REPETICIONES); do
        echo "    Repetición $i/$REPETICIONES"
        tiempo=$(./buffon_fork $LANZAMIENTOS $w | tail -n 1 | cut -d' ' -f3)
        echo "Fork,$w,$i,$tiempo" >> $ARCHIVO_SALIDA
    done
done

echo "Pruebas completadas. Resultados guardados en $ARCHIVO_SALIDA"