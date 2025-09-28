# Análisis de Paralelización - Cálculo de π usando Monte Carlo

## 1. Análisis de los Algoritmos

### Algoritmo de Monte Carlo para π
El algoritmo de Monte Carlo para calcular π se basa en el método de "dartboard" (tablero de dardos):

1. **Generación de puntos aleatorios**: Se generan puntos aleatorios (x, y) en un cuadrado unitario [0,1] × [0,1]
2. **Verificación de condición**: Se verifica si cada punto está dentro del círculo unitario (x² + y² < 1)
3. **Cálculo de π**: π ≈ 4 × (puntos_dentro_círculo / total_puntos)

### Características del Algoritmo
- **Embarrassingly Parallel**: Cada punto puede ser procesado independientemente
- **Alto paralelismo**: No hay dependencias entre cálculos
- **Escalabilidad**: Fácil de distribuir entre múltiples procesadores

## 2. Implementaciones Realizadas

### 2.1 Versión Serial (`Dart.c`)
```c
// Algoritmo básico sin paralelización
for (k = hits = 0; k < n; ++k) {
    double y = rand() * factor;
    double x = rand() * factor;
    if (x*x + y*y < 1.0)
        ++hits;
}
```

**Características:**
- Complejidad: O(n)
- Memoria: O(1)
- Sin overhead de paralelización

### 2.2 Versión con Hilos (`DartBoardH.c`)
```c
// Paralelización con pthreads
void *DartBoard(void *thread_id) {
    // Cada hilo procesa un chunk de puntos
    long local_hits = 0;
    for (long k = start; k < end; ++k) {
        // Cálculo independiente
    }
    // Sincronización con mutex
    pthread_mutex_lock(&mutex);
    hits += local_hits;
    pthread_mutex_unlock(&mutex);
}
```

**Características:**
- **Ventajas**: Compartir memoria, comunicación rápida
- **Desventajas**: Overhead de sincronización, contención de memoria
- **Sincronización**: Mutex para proteger variable compartida `hits`

### 2.3 Versión con Procesos (`DartBoardF.c`)
```c
// Paralelización con fork()
pid_t pid = fork();
if (pid == 0) {
    // Proceso hijo: cálculo independiente
    long local_hits = 0;
    // ... cálculo ...
    write(pipe, &local_hits, sizeof(long));
    exit(0);
} else {
    // Proceso padre: recolección de resultados
    read(pipe, &local_hits, sizeof(long));
    hits += local_hits;
}
```

**Características:**
- **Ventajas**: Aislamiento de memoria, no hay contención
- **Desventajas**: Overhead de creación de procesos, comunicación por pipes
- **Comunicación**: Pipes para transferir resultados

## 3. Análisis de Opciones de Paralelización

### 3.1 Estrategias de Paralelización

#### A. Paralelización por Chunks
- **División del trabajo**: n puntos divididos en chunks de tamaño n/p
- **Balanceo de carga**: Último proceso/hilo puede tener chunk más pequeño
- **Granularidad**: Chunk muy pequeño → overhead, chunk muy grande → desbalance

#### B. Distribución de Memoria
- **Hilos**: Memoria compartida, acceso directo a variables globales
- **Procesos**: Memoria aislada, comunicación explícita necesaria

### 3.2 Optimizaciones de CPU

#### A. Localidad de Referencia
```c
// Optimización: variables locales para reducir contención
long local_hits = 0;  // Variable local por hilo/proceso
// ... cálculos independientes ...
// Solo al final: sincronización/comunicación
```

#### B. Reducción de Sincronización
- **Hilos**: Mutex solo al final del cálculo
- **Procesos**: Comunicación solo al final

### 3.3 Optimizaciones de Memoria

#### A. Gestión de Memoria
- **Hilos**: Stack compartido, heap compartido
- **Procesos**: Stack independiente, heap independiente

#### B. Cache Performance
- **Hilos**: Posible false sharing si variables están en misma línea de cache
- **Procesos**: No hay false sharing, mejor aislamiento

## 4. Análisis de Resultados de Desempeño

### 4.1 Factores que Afectan el Rendimiento

#### A. Overhead de Paralelización
- **Creación de hilos**: ~1-10 μs
- **Creación de procesos**: ~100-1000 μs
- **Sincronización**: Mutex vs Pipes

#### B. Escalabilidad
- **Speedup teórico**: S(p) = T(1) / T(p)
- **Eficiencia**: E(p) = S(p) / p
- **Ley de Amdahl**: Limitaciones por código serial

#### C. Contención de Recursos
- **Hilos**: Contención en acceso a memoria compartida
- **Procesos**: Overhead de comunicación entre procesos

### 4.2 Comparación Hilos vs Procesos

| Aspecto | Hilos (pthreads) | Procesos (fork) |
|---------|------------------|-----------------|
| **Creación** | Rápida (~μs) | Lenta (~ms) |
| **Memoria** | Compartida | Aislada |
| **Comunicación** | Directa | Pipes/IPC |
| **Sincronización** | Mutex/Semáforos | Señales/Shared Memory |
| **Escalabilidad** | Limitada por contención | Mejor aislamiento |
| **Debugging** | Complejo | Más simple |

### 4.3 Predicciones de Rendimiento

#### Para N pequeño (< 1M puntos):
- **Serial** probablemente más rápido (sin overhead)
- **Hilos** con overhead significativo
- **Procesos** con overhead muy alto

#### Para N grande (> 10M puntos):
- **Hilos** deberían mostrar speedup con 2-4 hilos
- **Procesos** pueden ser competitivos con 2-4 procesos
- **Contención** puede limitar escalabilidad

#### Para muchos procesadores (> 8):
- **Procesos** pueden escalar mejor (menos contención)
- **Hilos** pueden sufrir más contención de memoria

## 5. Instrucciones de Uso

### Compilación
```bash
make all
```

### Ejecución de Pruebas
```bash
# Pruebas rápidas
make test

# Análisis completo
make benchmark

# Script de PowerShell (Windows)
.\benchmark.ps1
```

### Parámetros de Ejecución
```bash
# Serial
./dart_serial 1000000

# Hilos
./dart_threads 1000000 4

# Procesos
./dart_processes 1000000 4
```

## 6. Conclusiones Esperadas

1. **Serial vs Paralelo**: Para N pequeño, serial será más rápido
2. **Hilos vs Procesos**: Hilos más rápidos para pocos procesadores, procesos pueden escalar mejor
3. **Escalabilidad**: Speedup sublineal debido a overhead y contención
4. **Optimización**: Reducir sincronización y comunicación es clave

## 7. Métricas de Evaluación

- **Tiempo de ejecución**: Medición con `clock()`
- **Speedup**: S(p) = T(1) / T(p)
- **Eficiencia**: E(p) = S(p) / p
- **Escalabilidad**: Comportamiento con diferentes p
