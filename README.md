# Algoritmos Avanzados — PUCP

Repositorio de todos los ejercicios que vaya desarrollando en el curso de Algoritmos Avanzados (PUCP). Cada ejercicio es un mini proyecto en C++ independiente, gestionado con CMake (creados con CLion).

## Estructura del repositorio

```
.
├── BackTracking/
│   ├── 8_Rreinas/                   # Problema de las 8 reinas
│   ├── Lab01_2024_1/Pregunta_1/     # (pendiente)
│   ├── Lab01_2024_2/Pregunta_2/     # Ciclo Hamiltoniano
│   ├── Lab01_2026_1/Pregunta 1/     # Partición en dos subconjuntos (mínima diferencia)
│   └── Mochila_0_1/                 # Mochila 0/1 vía backtracking (pendiente)
├── Programacion_Dinamica/
│   ├── Cajero_Automatico/           # Mínima cantidad de monedas (coin change)
│   ├── Lab2_2026_1/
│   │   ├── Pregunta_1/              # Scheduling de eventos con bono por continuidad
│   │   └── Pregunta_2/              # Validación de actas (ONCE) con subset-sum
│   ├── Mochila_1_0/                 # Mochila 0/1 clásica (tabulación)
│   ├── Subiendo_Escaleras/          # Climbing stairs (equivalente a Fibonacci)
│   └── Varilla/                     # Rod cutting (memorización e iterativo)
├── Heuristicas/
│   ├── Lab3_2024_1/                 # Nearest neighbor; mochila fraccionaria (ganancia/peso)
│   └── Lab3_2026_1/                 # Regla de Smith; ruta con autonomía limitada
├── Metaheuristicas/
│   ├── 01_GRASP_Construccion_Mochila/     # GRASP (construcción): mochila 0/1
│   ├── 02_GRASP_Ruta_Repartidor/          # GRASP (construcción): ruta tipo TSP
│   ├── 03_GRASP_Estaciones_Bomberos/      # GRASP adaptativo: cobertura de conjuntos
│   ├── Lab04_2025_1/                      # GRASP: balanceo de líneas de producción (a/b)
│   └── Lab04_2026_1/                      # GRASP: selección de proyectos; bin packing
└── Algoritmos_Geneticos/
    ├── maximizar_f_x/                # AG: maximizar f(x) = x²
    └── Lab05_2025_2/                 # (pendiente)
```

Para el detalle de cada ejercicio (enunciado, estrategia de solución, complejidad y estado) revisa **[EJERCICIOS.md](EJERCICIOS.md)**.

## Requisitos

- Compilador con soporte de C++20 (los proyectos usan `set(CMAKE_CXX_STANDARD 20)`).
- CMake 3.x o superior (algunos `CMakeLists.txt` fueron generados por CLion con `cmake_minimum_required(VERSION 4.3)`; si tu CMake es más antiguo, basta bajar ese número).

## Cómo compilar y ejecutar un ejercicio

Cada carpeta de ejercicio es un proyecto CMake autocontenido. Por ejemplo, para compilar y correr "Varilla":

```bash
cd Programacion_Dinamica/Varilla
cmake -B build -S .
cmake --build build
./build/Varilla        # en Windows: build\Varilla.exe o build\Debug\Varilla.exe
```

El mismo patrón aplica para el resto de carpetas, cambiando el nombre del ejecutable según `add_executable(...)` en su `CMakeLists.txt`.

## Organización por tema

- **BackTracking/**: ejercicios resueltos por exploración exhaustiva con poda (backtracking).
- **Programacion_Dinamica/**: ejercicios resueltos con programación dinámica (top-down con memorización y/o bottom-up con tabulación).
- **Heuristicas/**: ejercicios resueltos con heurísticas voraces (greedy) y reglas de prioridad.
- **Metaheuristicas/**: ejercicios resueltos con GRASP (fase de construcción).
- **Algoritmos_Geneticos/**: ejercicios resueltos con Algoritmos Genéticos.
