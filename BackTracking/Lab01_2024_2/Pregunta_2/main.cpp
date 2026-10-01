#include <iostream>
#include <vector>

using namespace std;

/*
 * Lógica de la función 'esValido':
 * Se verifica si el vértice candidato 'v' puede ser insertado en la posición 'pos' del recorrido.
 * Se valida que exista una arista adyacente entre el vértice previo en el camino y 'v'.
 * Además, se itera sobre los vértices ya asignados en el camino para asegurar que 'v' no haya sido visitado.
 */
bool esValido(int v, int pos, const vector<int>& path, const vector<vector<int>>& grafo) {
    if (grafo[path[pos - 1]][v] == 0) {
        return false;
    }
    for (int i = 0; i < pos; i++) {
        if (path[i] == v) {
            return false;
        }
    }
    return true;
}

/*
 * Lógica de la función 'backtrackingHamiltoniano':
 * Emplea recursividad y backtracking para evaluar caminos posibles.
 * Caso base: Si la posición alcanza el número total de vértices, verifica si el último nodo conecta con el de origen para cerrar el ciclo Hamiltoniano.
 * Caso recursivo: Si no es el final, prueba insertar cada vértice disponible. Si es válido, lo agrega y llama a la función para el siguiente paso; si el camino falla, revierte la asignación (backtracking con path[pos] = -1) y prueba la siguiente opción.
 */
bool backtrackingHamiltoniano(const vector<vector<int>>& grafo, vector<int>& path, int pos, int V) {
    if (pos == V) {
        if (grafo[path[pos - 1]][path[0]] == 1) {
            return true;
        }
        return false;
    }

    for (int v = 1; v < V; v++) {
        if (esValido(v, pos, path, grafo)) {
            path[pos] = v; // Se asigna el vértice tentativo

            if (backtrackingHamiltoniano(grafo, path, pos + 1, V)) {
                return true;
            }

            path[pos] = -1; // Backtracking: se revierte la asignación si no conduce a solución
        }
    }
    return false;
}

/*
 * Lógica de la función 'resolverCicloHamiltoniano':
 * Inicializa el arreglo del recorrido (path) asignando el vértice de partida (0 que representa 'A').
 * Llama a la función booleana principal de backtracking y procesa su resultado[cite: 1].
 * Si retorna verdadero, imprime la secuencia de vértices que conforman el ciclo Hamiltoniano; de lo contrario, advierte que no existe.
 */
void resolverCicloHamiltoniano(const vector<vector<int>>& grafo) {
    int V = grafo.size();
    vector<int> path(V, -1);

    path[0] = 0; // Se inicia el recorrido en el vértice 0 (A)

    if (!backtrackingHamiltoniano(grafo, path, 1, V)) {
        cout << "No se puede formar ciclo Hamiltoniano en este grafo." << endl;
        return;
    }

    cout << "Ciclo Hamiltoniano encontrado: ";
    for (int i = 0; i < V; i++) {
        cout << char('A' + path[i]) << " -> ";
    }
    cout << char('A' + path[0]) << endl; // Regreso al origen para cerrar el ciclo
}

int main() {
    // Grafo de la Figura 1 (Con ciclo)
    // Vértices A, B, C, D, E y Aristas: A-B, B-C, C-D, D-E, E-A, A-C, A-D, B-E[cite: 1]
    cout << "--- Prueba Figura 1 ---" << endl;
    vector<vector<int>> grafo1 = {
        {0, 1, 1, 1, 1}, // A conecta con B, C, D, E
        {1, 0, 1, 0, 1}, // B conecta con A, C, E
        {1, 1, 0, 1, 0}, // C conecta con A, B, D
        {1, 0, 1, 0, 1}, // D conecta con A, C, E
        {1, 1, 0, 1, 0}  // E conecta con A, B, D
    };
    resolverCicloHamiltoniano(grafo1);

    // Grafo de la Figura 2 (Sin ciclo)
    // Contiene un vértice terminal que imposibilita el ciclo[cite: 1]
    cout << "\n--- Prueba Figura 2 ---" << endl;
    vector<vector<int>> grafo2 = {
        {0, 1, 0, 1, 0}, // A
        {1, 0, 1, 0, 0}, // B
        {0, 1, 0, 1, 1}, // C
        {1, 0, 1, 0, 0}, // D
        {0, 0, 1, 0, 0}  // E
    };
    resolverCicloHamiltoniano(grafo2);

    return 0;
}