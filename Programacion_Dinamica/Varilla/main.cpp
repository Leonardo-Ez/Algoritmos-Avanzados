#include <iomanip>
#include <iostream>
using namespace std;
#define L 4
#define MAX_MEMORIA 10

/*
 * Contexto: Este ejercicio sirve para comprender la programación dinamica
 * del tipo memorización para algoritmos recursivos.
 * Consiste en una varilla de longitud L que se puede dividir, cada longitud tiene un precio
 * independiente, debemos sacar el precio más optimo posible (máximo), podemos cortar la varilla
 * cuantas veces queramos pero solo en partes enteras y por obvias razones, no podemos cortar en
 * longitud de 0
 *
 * Se implementan ambos enfoques de DP para comparar:
 *   1) cortarVarillaRecursivo: top-down con memorización.
 *   2) cortarVarillaIterativo: bottom-up con tabulación.
 * Ambas resuelven la misma recurrencia:
 *   mejorValor(n) = max sobre i en [1,n] de (precios[i] + mejorValor(n - i))
 * Complejidad: O(L^2) tiempo, O(L) espacio, en ambas versiones.
 *
 */


//recursivo:
int cortarVarillaRecursivo(int *memoria,int longitud_varilla,int *precios) {
    //caso base
    if (longitud_varilla<=0) {
        return 0;
    }
    if (memoria[longitud_varilla] != 0) {//verificamos si el valor de la longitud actual exista en la memoria
        return memoria[longitud_varilla];//si existe retornamos ese valor
    }

    int mejor_valor=0;//inicializamos variables para este caso
    int valor=0;
    //recorremos todas las posibilidades
    for (int i=1;i<=longitud_varilla;i++) {
        //calculamos el valor que es el precio del corte actual + el precio max del resto faltante (longitud_varilla-i)
        valor = precios[i]+cortarVarillaRecursivo(memoria,longitud_varilla-i,precios);
        if (valor>mejor_valor) {//si el precio actual es mayor que el precio mejor precio anterior
            mejor_valor = valor;//cambiamos el valor del mejor valor
        }
    }

    memoria[longitud_varilla] = mejor_valor; //guardamos el valor en la memoria
    return mejor_valor;

}

//iterativo:

int cortarVarillaIterativo(int longitud_varilla,int *precios) {
    int tabla[MAX_MEMORIA]{};
    //inicializamos los valores en una tabla que sirve de memoria

    //recorremos todos desde el más pequeño al más grande
    for (int varillaActual=1;varillaActual<=longitud_varilla;varillaActual++) {
        int max_valor=0;
        for (int i=1;i<=varillaActual;i++) {
            int valor=precios[i]+tabla[varillaActual-i];
            if (valor>max_valor) {
                max_valor = valor;
            }
        }
        tabla[varillaActual] = max_valor;
    }

    return tabla[longitud_varilla];

}


int main() {
    //valores iniciales:
    int precios[12]{0,1,5,8,9,10,17,17,20,25,30};
    int varilla=L;
    //inicializamos la memoria en 0
    //este es el componente que hace que sea dinámico
    //memorización
    int memoria[MAX_MEMORIA]{};

    //lanzamos la función recursiva
    cout<<cortarVarillaRecursivo(memoria, varilla,precios)<<endl;
    cout<<cortarVarillaIterativo(varilla,precios)<<endl;


}
