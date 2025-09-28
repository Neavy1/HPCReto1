# Script de análisis de rendimiento para el cálculo de π
# usando algoritmos de Monte Carlo con diferentes estrategias de paralelización

Write-Host "=========================================="
Write-Host "ANÁLISIS DE RENDIMIENTO - CÁLCULO DE π"
Write-Host "=========================================="
Write-Host ""

# Compilar todos los programas
Write-Host "Compilando programas..."
& make clean
& make all

if ($LASTEXITCODE -ne 0) {
    Write-Host "Error al compilar los programas"
    exit 1
}

Write-Host "Compilación exitosa!"
Write-Host ""

# Parámetros de prueba
$ITERATIONS = 10000000
$THREADS_VALUES = @(1, 2, 4, 8)
$PROCESSES_VALUES = @(1, 2, 4, 8)

# Función para ejecutar y medir tiempo
function Run-Test {
    param(
        [string]$program,
        [string]$args,
        [string]$description
    )
    
    Write-Host "Ejecutando: $description"
    Write-Host "Comando: $program $args"
    
    # Ejecutar 3 veces y tomar el promedio
    $totalTime = 0
    for ($i = 1; $i -le 3; $i++) {
        $output = & $program $args.Split(' ')
        $timeValue = [regex]::Match($output, '(\d+\.\d+) segundos').Groups[1].Value
        if ($timeValue) {
            $totalTime += [double]$timeValue
        }
    }
    $avgTime = $totalTime / 3
    Write-Host "Tiempo promedio: $([math]::Round($avgTime, 4)) segundos"
    Write-Host "---"
}

Write-Host "=========================================="
Write-Host "VERSIÓN SERIAL"
Write-Host "=========================================="
Run-Test ".\dart_serial.exe" "$ITERATIONS" "Versión serial"

Write-Host ""
Write-Host "=========================================="
Write-Host "VERSIÓN CON HILOS (PTHREADS)"
Write-Host "=========================================="
foreach ($threads in $THREADS_VALUES) {
    Run-Test ".\dart_threads.exe" "$ITERATIONS $threads" "Hilos: $threads"
}

Write-Host ""
Write-Host "=========================================="
Write-Host "VERSIÓN CON PROCESOS (FORK)"
Write-Host "=========================================="
foreach ($processes in $PROCESSES_VALUES) {
    Run-Test ".\dart_processes.exe" "$ITERATIONS $processes" "Procesos: $processes"
}

Write-Host ""
Write-Host "=========================================="
Write-Host "ANÁLISIS COMPLETADO"
Write-Host "=========================================="
Write-Host ""
Write-Host "Para un análisis más detallado, ejecuta:"
Write-Host "make benchmark"
Write-Host ""
Write-Host "Para pruebas rápidas, ejecuta:"
Write-Host "make test"
