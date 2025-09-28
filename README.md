# Reto 1 - Paralelización con Hilos y Procesos

## Descripción del Proyecto

Este proyecto implementa el cálculo de π usando el algoritmo de Monte Carlo con diferentes estrategias de paralelización:

1. **Versión Serial** (`Dart.c`)
2. **Versión con Hilos** (`DartBoardH.c`) - usando pthreads
3. **Versión con Procesos** (`DartBoardF.c`) - usando fork()

## Archivos del Proyecto

- `Dart.c` - Implementación serial del algoritmo de Monte Carlo
- `DartBoardH.c` - Implementación paralela con hilos (pthreads)
- `DartBoardF.c` - Implementación paralela con procesos (fork)
- `Makefile` - Script de compilación para sistemas Unix/Linux
- `compile.bat` - Script de compilación para Windows
- `benchmark.ps1` - Script de análisis de rendimiento para PowerShell
- `ANALISIS_PARALELIZACION.md` - Documentación técnica completa

## Compilación

### En Windows (Visual Studio)
```cmd
compile.bat
```

### En Linux/Unix (GCC)
```bash
make all
```

### Compilación Manual
```bash
# Serial
gcc -Wall -Wextra -O2 -std=c99 -o dart_serial Dart.c -lm

# Hilos
gcc -Wall -Wextra -O2 -std=c99 -o dart_threads DartBoardH.c -pthread -lm

# Procesos
gcc -Wall -Wextra -O2 -std=c99 -o dart_processes DartBoardF.c -lm
```

## Ejecución

### Parámetros de Entrada
- **Serial**: `./dart_serial <numero_iteraciones>`
- **Hilos**: `./dart_threads <numero_iteraciones> <numero_hilos>`
- **Procesos**: `./dart_processes <numero_iteraciones> <numero_procesos>`

### Ejemplos de Uso
```bash
# Versión serial con 1 millón de iteraciones
./dart_serial 1000000

# Versión con hilos: 1 millón de iteraciones, 4 hilos
./dart_threads 1000000 4

# Versión con procesos: 1 millón de iteraciones, 4 procesos
./dart_processes 1000000 4
```

## Análisis de Rendimiento

### Ejecutar Análisis Completo
```bash
# En Linux/Unix
make benchmark

# En Windows PowerShell
.\benchmark.ps1
```

### Pruebas Rápidas
```bash
# En Linux/Unix
make test

# En Windows
dart_serial.exe 1000000
dart_threads.exe 1000000 4
dart_processes.exe 1000000 4
```

## Algoritmo de Monte Carlo

El algoritmo implementado calcula π usando el método de "dartboard":

1. **Generación de puntos aleatorios** en un cuadrado unitario [0,1] × [0,1]
2. **Verificación** si cada punto está dentro del círculo unitario (x² + y² < 1)
3. **Cálculo de π**: π ≈ 4 × (puntos_dentro_círculo / total_puntos)

## Características de Paralelización

### Versión con Hilos (pthreads)
- **Ventajas**: Memoria compartida, comunicación rápida
- **Desventajas**: Contención de memoria, overhead de sincronización
- **Sincronización**: Mutex para proteger variables compartidas

### Versión con Procesos (fork)
- **Ventajas**: Aislamiento de memoria, mejor escalabilidad
- **Desventajas**: Overhead de creación de procesos, comunicación por pipes
- **Comunicación**: Pipes para transferir resultados entre procesos

## Optimizaciones Implementadas

1. **Reducción de contención**: Variables locales por hilo/proceso
2. **Minimización de sincronización**: Mutex/comunicación solo al final
3. **Balanceo de carga**: Distribución equitativa del trabajo
4. **Gestión de memoria**: Optimización de acceso a memoria

## Resultados Esperados

### Factores de Rendimiento
- **Speedup**: Mejora de tiempo con paralelización
- **Eficiencia**: Speedup / número de procesadores
- **Escalabilidad**: Comportamiento con diferentes números de procesadores

### Comparación Hilos vs Procesos
- **Hilos**: Mejor para pocos procesadores, menor overhead
- **Procesos**: Mejor para muchos procesadores, menos contención

## Documentación Técnica

Para un análisis detallado de la paralelización, optimizaciones y resultados esperados, consulta:
- `ANALISIS_PARALELIZACION.md` - Análisis técnico completo

## Requisitos del Sistema

- **Compilador**: GCC (Linux) o Visual Studio (Windows)
- **Librerías**: pthread (para hilos), math (para cálculos)
- **Sistema Operativo**: Linux, Windows, o macOS

## Troubleshooting

### Problemas Comunes

1. **Error de compilación con hilos**:
   ```bash
   gcc -pthread -o dart_threads DartBoardH.c
   ```

2. **Error de compilación en Windows**:
   - Asegúrate de tener Visual Studio instalado
   - Usa `compile.bat` para compilación automática

3. **Problemas de rendimiento**:
   - Usa números grandes de iteraciones (> 1M) para ver diferencias
   - Prueba con diferentes números de hilos/procesos

## Autores

Implementación para el curso de HPC - Reto 1
