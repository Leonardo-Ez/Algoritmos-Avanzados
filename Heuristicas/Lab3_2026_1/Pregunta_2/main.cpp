#include <iomanip>
#include <iostream>
#include <list>
using namespace std;
#define NUM_CIUDADES 5

struct Ciudad {
    int posCiudad;
    int distancia;
};

void imprimirMatriz(int matrizAdyaciencia[][NUM_CIUDADES],
    const char *textoAdicional) {
    cout<<textoAdicional<<endl;

    for (int i = 0; i < NUM_CIUDADES; i++) {
        for (int j = 0; j < NUM_CIUDADES; j++) {
            cout << setw(6)<<matrizAdyaciencia[i][j];
        }
        cout << endl;
    }
    cout << endl;
}

bool visitado(const list <int>&listaCiudades,int i) {
    for (int ciudad: listaCiudades) {
        if (ciudad == i) return true;
    }
    return false;
}

void buscarRutaGreedy(int matrizAdyaciencia[][NUM_CIUDADES],int tanque,
        const int *ciudadesConGrifo,int inicio,int fin) {
    int tanqueCopia=10;
    int salida=0;
    list<int> listaCiudades;
    listaCiudades.push_back(inicio);
    while (true) { //la iteracion no termina hasta que lleguemos
        Ciudad ciudadMasLejana{};
        if (ciudadesConGrifo[inicio]==1) {
            tanque=tanqueCopia;
        }
        for (int i = 0; i < NUM_CIUDADES; i++) {
            //recorremos toda la fila encontrando el mayor
            if (ciudadMasLejana.distancia<matrizAdyaciencia[inicio][i]
                and !visitado(listaCiudades,i)
            ) {
                ciudadMasLejana.posCiudad = i;
                ciudadMasLejana.distancia = matrizAdyaciencia[inicio][i];
                /*cada que encontremos uno mayor guardamos en el struct
                    si o si recorremos todas las ciudades, pues no están ordenadas
                */
            }

        }

        if (ciudadMasLejana.distancia > tanque) {
            salida=-1;
            break;
        }
        else {
            listaCiudades.push_back(ciudadMasLejana.posCiudad);
        }
        if (ciudadMasLejana.posCiudad == fin) break;

        tanque-=ciudadMasLejana.distancia;
        inicio=ciudadMasLejana.posCiudad;
    }

    cout<<"Ruta Greedy obtenida:"<<endl;

    for (auto ciudad : listaCiudades) {
        cout << setw(6)<<ciudad;
    }
    cout<<endl;
    if (salida == -1) cout <<"Resultado: NO SE PUEDE LLEGAR AL DESTINO"<<endl;

}


int main() {

    // int matrizAdyaciencia[NUM_CIUDADES][NUM_CIUDADES] {
    //     {0,4,8,5,0,0},
    //     {4,0,3,2,6,0},
    //     {8,3,0,4,7,0},
    //     {5,2,4,0,3,4},
    //     {0,6,7,3,0,9},
    //     {0,0,0,4,9,0}
    // };
    int matrizAdyaciencia[NUM_CIUDADES][NUM_CIUDADES] {
        {0,4,7,5,0},
        {4,0,3,2,6},
        {7,3,0,4,7},
        {5,2,4,0,3},
        {0,6,7,3,0}
    };

    int ciudadesConGrifo[NUM_CIUDADES]{1,0,1,0,0};

    imprimirMatriz(matrizAdyaciencia,"Matriz Adyaciencia");

    buscarRutaGreedy(matrizAdyaciencia,10,
        ciudadesConGrifo,0,1);


    return 0;
}