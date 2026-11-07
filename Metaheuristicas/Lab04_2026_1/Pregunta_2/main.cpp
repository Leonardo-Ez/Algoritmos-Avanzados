#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

/*
 * LABORATORIO 4 (2026-1) - Pregunta 2
 * Algoritmos Avanzados - PUCP
 *
 * Autoría:
 *   - Enunciado: Laboratorio 4 del curso (2026-1).
 *   - Implementación: Hector Leonardo Bonifacio Espinoza :).
 *
 * Problema: repartir los pedidos de MediExpress S.A.C en camiones de
 * capacidad K = 50 kg, minimizando la cantidad de camiones (bin packing).
 *
 * Estrategia: GRASP construcción (alfa = 0.2, 1000 iteraciones).
 *   - Mérito: el peso del pedido (se MAXIMIZA: primero los pesados y luego
 *     se rellena con los livianos), aunque el objetivo minimiza camiones.
 *   - Los pedidos se ordenan una sola vez de mayor a menor peso; así la RCL
 *     son los primeros k del vector (los que pasan el umbral) y la posición
 *     elegida en la RCL es la misma en el vector de pedidos.
 *   - El pedido elegido va al primer camión donde quepa (First Fit); si no
 *     cabe en ninguno, se abre un camión nuevo.
 *
 * Resultado: D E | B F | C A = 3 camiones, igual al ejemplo del enunciado.
 * Es el óptimo: los pedidos suman 135 kg y 135 / 50 = 2.7, así que con
 * 2 camiones es imposible.
 */
#define CAPACIDAD 50
#define ITERACIONES 1000
#define ALFA 0.2

struct Pedido {
    char pedido;
    int peso;
};

struct Camion {
    vector<Pedido> pedidos;
    int pesoTotal;
};

bool compararPedidos(Pedido &p1, Pedido &p2) {
    return p1.peso > p2.peso;
}


vector<Pedido> pedidosSobreUmbral(const vector<Pedido> &pedidosActuales,double umbral) {
    vector<Pedido> sobreUmbral;
    for (auto &pedido : pedidosActuales) {
        if (pedido.peso >= umbral) {
            sobreUmbral.push_back(pedido);
        }
    }
    return sobreUmbral;
}

void asignarACamion(const Pedido &pedido,int capacidad,
    vector<Camion> &camiones,int &cantidad) {
    //recorremos todos por que al tener un componente aleatorio
    //no sabemos si queda espacio en alguno de los camiones anteriores
    for (auto &camion : camiones) {
        if (camion.pesoTotal+pedido.peso <= capacidad) {
            camion.pedidos.push_back(pedido);
            camion.pesoTotal += pedido.peso;
            return;
        }
    }
    //si llegó aquí significa que no se asignó a un camión ya existente

    Camion camion{};
    camion.pedidos.push_back(pedido);
    camion.pesoTotal += pedido.peso;

    camiones.push_back(camion);
    cantidad++;
}

void quitarUsado(vector<Pedido> &pedidosActuales,char pedidoID) {
    vector<Pedido> copiaPedidosActuales;
    for (auto &pedido : pedidosActuales) {
        if (pedido.pedido != pedidoID) {
            copiaPedidosActuales.push_back(pedido);
        }
    }
    pedidosActuales = copiaPedidosActuales;
}

void imprimirSolucion(int numPedidos,
    int copiaIteraciones,
    const vector<Camion> &mejorSeleccionCamiones,
    int menorCantidad) {
    cout<<"============================================="<<endl;
    cout<<"GRASP CONSTRUCCION - MEDIEXPRESS S.A.C"<<endl;
    cout<<"============================================="<<endl;
    cout<<"Pedidos: "<<numPedidos<<endl;
    cout<<"Alfa: "<<ALFA<<" Iteraciones: "<<copiaIteraciones<<endl;
    cout<<"Capacidad de carga por camión (K): "<< CAPACIDAD<<" kg"<<endl;
    cout<<"============================================="<<endl;
    int i=0;
    for (auto &camion : mejorSeleccionCamiones) {
        cout<<"Camion "<<i+1 <<"(cap="<< CAPACIDAD<<"kg):";
        for (auto &pedido : camion.pedidos) {
            cout<<pedido.pedido<<' ';
        }
        cout<<" |Peso: "<<camion.pesoTotal<<"/"<< CAPACIDAD<<"kg"<<endl;
        i++;
    }
    cout<<"Cantidad total de camiones:"<<menorCantidad<<endl;
}

int encontrarCuantosPasanUmbral(vector<Pedido> &pedidosActuales,double umbral) {
    int i=0;
    for (auto &pedido : pedidosActuales) {
        if (pedido.peso<umbral) {
            break;
        }
        i++;
    }
    return i;
}

void asignarACamiones(vector<Pedido> pedidos,int capacidad,int iteraciones) {
    //ordenamos los pedidos
    sort(pedidos.begin(),pedidos.end(),compararPedidos);
    int copiaIteraciones = iteraciones;

    vector<Camion> mejorSeleccionCamiones{};
    int menorCantidad=INT_MAX;

    while (iteraciones) {
        vector<Pedido> pedidosActuales=pedidos;
        vector<Camion> camiones{};
        int cantidad=0;
        while(!pedidosActuales.empty()) {
            int beta=pedidosActuales.front().peso;
            int tau=pedidosActuales.back().peso;

            double umbral=beta-ALFA*(beta-tau);

            //vector<Pedido> sobreUmbral=pedidosSobreUmbral(pedidosActuales,umbral);
            int sobreUmbral=encontrarCuantosPasanUmbral(pedidosActuales,umbral);
            int pos=rand()%sobreUmbral;

            //quitarUsado(pedidosActuales,sobreUmbral[pos].pedido);
            asignarACamion(pedidosActuales[pos],capacidad,camiones,cantidad);
            pedidosActuales.erase(pedidosActuales.begin()+pos);
        }
        if (cantidad<menorCantidad) {
            menorCantidad=cantidad;
            mejorSeleccionCamiones=camiones;
        }

        iteraciones--;
    }
    imprimirSolucion(pedidos.size(), copiaIteraciones, mejorSeleccionCamiones, menorCantidad);
}



int main() {
    srand(time(NULL));
    vector<Pedido> pedidos{
        {'A',15},
        {'B',32},
        {'C',20},
        {'D',42},
        {'E',8},
        {'F',18}
    };

    asignarACamiones(pedidos,CAPACIDAD,ITERACIONES);



}
