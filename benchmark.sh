#!/bin/bash

# Script de análisis de rendimiento para el cálculo de π
# usando algoritmos de Monte Carlo con diferentes estrategias de paralelización

echo "=========================================="
echo "ANÁLISIS DE RENDIMIENTO - CÁLCULO DE π"
echo "=========================================="
echo ""

# Compilar todos los programas
echo "Compilando programas..."
make clean
make all

if [ $? -ne 0 ]; then
    echo "Error al compilar los programas"
    exit 1
fi

echo "Compilación exitosa!"
echo ""

# Parámetros de prueba
ITERATIONS=10000000
THREADS_VALUES=(1 2 4 8)
PROCESSES_VALUES=(1 2 4 8)

# Función para ejecutar y medir tiempo
run_test() {
    local program=$1
    local args=$2
    local description=$3
    
    echo "Ejecutando: $description"
    echo "Comando: $program $args"
    
    # Ejecutar 3 veces y tomar el promedio
    total_time=0
    for i in {1..3}; do
        time_output=$(./$program $args 2>&1)
        time_value=$(echo "$time_output" | grep -o '[0-9]*\.[0-9]* segundos' | grep -o '[0-9]*\.[0-9]*')
        if [ ! -z "$time_value" ]; then
            total_time=$(echo "$total_time + $time_value" | bc -l)
        fi
    done
    avg_time=$(echo "scale=4; $total_time / 3" | bc -l)
    echo "Tiempo promedio: ${avg_time} segundos"
    echo "---"
}

echo "=========================================="
echo "VERSIÓN SERIAL"
echo "=========================================="
run_test "dart_serial" "$ITERATIONS" "Versión serial"

echo ""
echo "=========================================="
echo "VERSIÓN CON HILOS (PTHREADS)"
echo "=========================================="
for threads in "${THREADS_VALUES[@]}"; do
    run_test "dart_threads" "$ITERATIONS $threads" "Hilos: $threads"
done

echo ""
echo "=========================================="
echo "VERSIÓN CON PROCESOS (FORK)"
echo "=========================================="
for processes in "${PROCESSES_VALUES[@]}"; do
    run_test "dart_processes" "$ITERATIONS $processes" "Procesos: $processes"
done

echo ""
echo "=========================================="
echo "ANÁLISIS COMPLETADO"
echo "=========================================="
echo ""
echo "Para un análisis más detallado, ejecuta:"
echo "make benchmark"
echo ""
echo "Para pruebas rápidas, ejecuta:"
echo "make test"
