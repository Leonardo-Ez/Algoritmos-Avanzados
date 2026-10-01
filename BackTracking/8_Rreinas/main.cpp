#include <iostream>
#include <iomanip>
using namespace std;
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
