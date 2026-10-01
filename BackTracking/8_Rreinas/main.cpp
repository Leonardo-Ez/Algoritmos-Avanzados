#include <iostream>
#include <iomanip>
using namespace std;

/*
 * Problema: colocar 8 reinas en un tablero de 8x8 tal que ninguna
 * ataque a otra (misma fila, columna o diagonal).
 *
 * Estrategia: backtracking columna por columna. Para cada columna se
 * prueba colocar la reina en cada fila; si esSeguro() lo permite, se
 * avanza recursivamente a la siguiente columna. Si ninguna fila de la
 * columna actual lleva a una solución completa, se deshace la última
 * colocación (tablero[fila][col] = 0) y se retrocede a probar otra
 * fila en la columna anterior.
 *
 * Complejidad: exponencial en el peor caso (acotada en la práctica
 * por la poda de esSeguro).
 */
#define MAX_X 8
#define MAX_Y 8
#define MAX_REINAS 8

void imprimirTablero(int tablero[][MAX_Y]) {
    for (int i = 0; i < MAX_X; i++) {
        for (int j = 0; j < MAX_Y; j++) {
            cout <<setw(5)<<tablero[i][j];
        }
        cout << endl;
        cout << endl;
    }
}

bool esSeguro(int tablero[][MAX_Y],int fila, int col) {
    //revisar la fila hacia la izquierda
    for (int i=0; i < col; i++) {
        if (tablero[fila][i]==1)return false;
    }

    //revisar la diagonal superior izquierda
    for (int i=fila,j=col;i>=0 && j>=0; i--,j--) {
        if (tablero[fila][i]==1)return false;
    }

    //revisar la diagonal inferior izquierda
    for (int i=fila,j=col;i<MAX_X && j>=0; i++,j--) {
        if (tablero[fila][i]==1)return false;
    }

    return true;//si pasa todas las pruebas, ok
}


bool solve(int tablero[][MAX_Y], int col) {

    if (col>=MAX_REINAS) {
        return true;
    }

    for (int fila=0; fila<MAX_X; fila++) {
        if (esSeguro(tablero,fila,col)) {
            tablero[fila][col]=1;
            if (solve(tablero,col+1)) {
                return true;
            }
        }

        tablero[fila][col]=0;
    }


    return false;
}






int main() {

    int tablero[MAX_X][MAX_Y]{}; //tablero inicializado en 0

    imprimirTablero(tablero);

    int cantidadReinasPorColocar=MAX_REINAS;

    solve(tablero,0);
    cout<<endl;
    cout<<endl;
    cout<<endl;
    cout<<endl;
    cout<<endl;
    imprimirTablero(tablero);

    return 0;
}
