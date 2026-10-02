#include <iomanip>
#include <iostream>
using namespace std;
#define NUM_NODOS 8
struct Nodo {
    int distancia;
    int pos;
};

void convertirMayusculas(int &inicio,int &fin) {
    if (inicio>='a' and inicio<='z') inicio+='A'-'a';
    if (fin>='a' and fin<='z') fin+='A'-'a';
}

void encontrarCamino(int matriz[][NUM_NODOS],int inicio,int fin) {
    convertirMayusculas(inicio,fin);
    /*reacomodamos el valor de inicio y fin para que entren en la matriz*/
    inicio = inicio-'A';
    fin = fin-'A';
    Nodo siguientenNodo{};
    int tiempoViaje=0;
    while (true) {
        siguientenNodo.distancia = INT_MAX;
        for(int i=0;i<NUM_NODOS;i++) {
            //recorremos horizontalmente
            if (inicio != i and matriz[inicio][i]!=0
                and matriz[inicio][i]<siguientenNodo.distancia) {
                    siguientenNodo.distancia = matriz[inicio][i];
                    siguientenNodo.pos = i;
            }
        }

        //a este punto encontramos el nodo más cercano o no existe camino para continuar
        if (siguientenNodo.distancia == INT_MAX and inicio != fin) {
            //el nodo cercano no existe,
            //pues siguienteNodo.distancia no cambió su valor
            cout<<"No se encontró la solución"<<endl;
            break;
        }
        tiempoViaje+=siguientenNodo.distancia;
        inicio = siguientenNodo.pos;//nos movemos a ese nodo
        if (inicio==fin) {
            cout<<"Tiempo de viaje "<<tiempoViaje<<" min"<<endl;
            break;
        }
    }






}

int main() {

    int matriz[NUM_NODOS][NUM_NODOS] {
        {0,4,5,6,0,0,0,0},
        {0,0,0,0,2,0,0,0},
        {0,0,0,0,0,0,0,3},
        {0,0,0,0,0,3,0,0},
        {0,0,0,0,0,0,10,0},
        {0,0,0,0,0,0,2,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0}
    };

    encontrarCamino(matriz,'a','g');

}