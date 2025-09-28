CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
LDFLAGS = -pthread -lm

# Nombres de los ejecutables
SERIAL = dart_serial
THREADS = dart_threads
PROCESSES = dart_processes

# Archivos fuente
SERIAL_SRC = Dart.c
THREADS_SRC = DartBoardH.c
PROCESSES_SRC = DartBoardF.c

# Reglas de compilación
all: $(SERIAL) $(THREADS) $(PROCESSES)

$(SERIAL): $(SERIAL_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(THREADS): $(THREADS_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(PROCESSES): $(PROCESSES_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Regla para limpiar archivos compilados
clean:
	rm -f $(SERIAL) $(THREADS) $(PROCESSES)

# Regla para ejecutar pruebas
test: all
	@echo "=== Ejecutando versión serial ==="
	./$(SERIAL) 1000000
	@echo ""
	@echo "=== Ejecutando versión con hilos (4 hilos) ==="
	./$(THREADS) 1000000 4
	@echo ""
	@echo "=== Ejecutando versión con procesos (4 procesos) ==="
	./$(PROCESSES) 1000000 4

# Regla para análisis de rendimiento
benchmark: all
	@echo "=== Análisis de Rendimiento ==="
	@echo "Iteraciones: 10,000,000"
	@echo ""
	@echo "Serial:"
	./$(SERIAL) 10000000
	@echo ""
	@echo "Hilos (2):"
	./$(THREADS) 10000000 2
	@echo ""
	@echo "Hilos (4):"
	./$(THREADS) 10000000 4
	@echo ""
	@echo "Hilos (8):"
	./$(THREADS) 10000000 8
	@echo ""
	@echo "Procesos (2):"
	./$(PROCESSES) 10000000 2
	@echo ""
	@echo "Procesos (4):"
	./$(PROCESSES) 10000000 4
	@echo ""
	@echo "Procesos (8):"
	./$(PROCESSES) 10000000 8

.PHONY: all clean test benchmark
