#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <cmath>
#include <algorithm>
#define ITERACIONES 50
#define NUMIND 10
#define TSELECCION 0.5
#define PCASAMIENTO 0.4
#define TMUTACION 0.3
#define NUM_SERVIDORES 4
#define MAX_CARGA 25

using namespace  std;



int calcularCargaMasAlta(const vector<int> &ind,const vector<int> &tiempos) {
    vector<int> cargas;
    for (int i=0;i<NUM_SERVIDORES;i++) {
        cargas.push_back(0);
    }
    for (int i=0;i<ind.size();i++) {
        cargas[ind[i]]+=tiempos[i];
    }
    int mayorCarga=0;
    for (int i=0;i<cargas.size();i++) {
        if (cargas[i]>mayorCarga) mayorCarga=cargas[i];
    }
    return mayorCarga;
}

int calcularCargaMasBaja(const vector<int> &ind,const vector<int> &tiempos) {
    vector<int> cargas;
    for (int i=0;i<NUM_SERVIDORES;i++) {
        cargas.push_back(0);
    }
    for (int i=0;i<ind.size();i++) {
        cargas[ind[i]]+=tiempos[i];
    }
    int menorCarga=INT_MAX;
    for (int i=0;i<cargas.size();i++) {
        if (cargas[i]<menorCarga) menorCarga=cargas[i];
    }
    return menorCarga;
}

int calculafitness(const vector<int> &cromo,
    const vector<int> &tiempos) {
    int cargaMasAlta=calcularCargaMasAlta(cromo,tiempos);
    int cargaMasBaja=calcularCargaMasBaja(cromo,tiempos);
    if (cargaMasAlta>MAX_CARGA) return cargaMasAlta;
    return cargaMasAlta-cargaMasBaja;
}

bool aberracion(const vector<int> &ind,
    const vector<int> &tiempos,int maxCarga) {
    for (int i=0;i<ind.size();i++) {
        if (ind[i]<0 or ind[i]>NUM_SERVIDORES-1) return true;
    }
    return false;
}

void muestrapoblacion(vector<vector<int>>poblacion,
    const vector<int> &tiempos) {
    for(int i=0;i<poblacion.size();i++) {
        for(int j=0;j<poblacion[i].size();j++) {
            cout<<poblacion[i][j]+1<<" ";
        }
        cout<<" diferencia maxima= "<< calculafitness(poblacion[i],tiempos)<<endl;
    }

}

int decimal(vector<int>ind) {
    int num=0;
    for (int i=0;i<ind.size();i++)
        num+=ind[i]*pow(2,i);
    return num;
}

void mataclon(vector<vector<int>> &poblacion) {
    map<int,vector<int>> indunicos;

    for(int i=0;i<poblacion.size();i++) {
        int dec=decimal(poblacion[i]);
        indunicos[dec]=poblacion[i];
    }
    poblacion.clear();
    for(map<int,vector<int>>::iterator it=indunicos.begin();
        it!=indunicos.end();it++) {
        poblacion.push_back(it->second);
    }
}


void generapoblacion(vector<vector<int>> &poblacion,
    const vector<int> &tiempos,int maxCarga) {
    int cont=0;
    srand(time(NULL));
    while (cont<NUMIND) {
        vector<int> individuo;
        // aqui también cambiar
        for (int i=0; i<tiempos.size(); i++)
            individuo.push_back(rand()%(NUM_SERVIDORES));
        if (!aberracion(individuo,tiempos,maxCarga)) {
            poblacion.push_back(individuo);
            cont++;
        }
    }
}

void calculasupervivencia(vector<vector<int>>poblacion,
    vector<int> &supervivencia,const vector<int> &tiempos) {
    int sumafitness=0;

    for(int i=0;i<poblacion.size();i++)
        sumafitness+=calculafitness(poblacion[i],tiempos);
    for (int i=0;i<poblacion.size();i++) {
        int superfit;
        superfit=round(100*(double)(sumafitness-calculafitness(poblacion[i],tiempos))
            /(double)((poblacion.size()-1)*sumafitness));
        supervivencia.push_back(superfit);
    }
}
void cargaruleta(vector<int >supervivencia,int *ruleta) {
    int cont=0;
    for (int i=0;i<supervivencia.size();i++)
        for (int j=0;j<supervivencia[i];j++) {
            ruleta[cont++]=i;
        }
}



void seleccion(vector<vector<int>>poblacion,vector<vector<int>> &padres,
   const vector<int> &tiempos ) {
    int ruleta[100]{-1};
    vector<int>supervivencia;
    calculasupervivencia(poblacion,supervivencia,tiempos);
    cargaruleta(supervivencia,ruleta);
    int npadres=round(poblacion.size()*TSELECCION);

    for (int i=0;i<npadres;i++) {
        int ticket=rand()%100;
        if (ruleta[ticket]!=-1)
            padres.push_back(poblacion[ruleta[ticket]]);
    }
    //muestrapoblacion(padres,paq);
}

void creahijo(vector<int>padre,vector<int>madre,
    vector<int>&hijo) {
    int posi=round(padre.size()*PCASAMIENTO);

    for (int i=0;i<posi;i++)
        hijo.push_back(padre[i]);
    for (int i=posi;i<madre.size();i++)
        hijo.push_back(madre[i]);
}

void casamiento(vector<vector<int>> &poblacion,
    vector<vector<int>>padres,
    const vector<int> &tiempos,int maxCarga) {

    for(int i=0;i<padres.size();i++)
        for(int j=0;j<padres.size();j++) {
            if (i!=j) {
                vector<int>hijo;
                creahijo(padres[i],padres[j],hijo);
                if (!aberracion(hijo,tiempos,maxCarga))
                    poblacion.push_back(hijo);
            }


        }
}


void regenerapoblacion(vector<vector<int>> &poblacion,
        const vector<int> &tiempos){
    mataclon(poblacion);
    // se evita emplear variables globales
    sort(poblacion.begin(), poblacion.end(),
         [tiempos](const vector<int>& a, const vector<int>& b) {
            int suma=0, sumb=0;
            for(int i=0;i<a.size();i++)
                suma += calculafitness(a, tiempos);
            for(int i=0;i<b.size();i++)
                sumb += calculafitness(b, tiempos);

            return suma < sumb;
         });
    if (poblacion.size()>NUMIND)
        poblacion.erase(poblacion.begin()+NUMIND,poblacion.end());

}

void mutacion(vector<vector<int>> &poblacion,vector<vector<int>>padres,
    const vector<int> &tiempos,int maxCarga) {
    int nmutaciones=round(padres[0].size()*TMUTACION);
    for (int i=0;i<padres.size();i++) {
        int cont=0;
        while (cont<nmutaciones) {
            int gen=rand()%padres[i].size();
            //aqui hay que cambiar si son numeros enteros
            while (true) {
                int nuevoGen=rand()%(NUM_SERVIDORES);
                if (nuevoGen!=padres[i][gen]) padres[i][gen]=nuevoGen;break;
            }
            cont++;
        }
        if (!aberracion(padres[i],tiempos,maxCarga))
            poblacion.push_back(padres[i]);
    }
}

void inversion(vector<vector<int>> &poblacion,vector<vector<int>>padres,
    const vector<int> &tiempos,int maxCarga) {

    for (int i=0;i<padres.size();i++) {
        for (int j=0;j<padres[i].size();j++) {
            //cambiar para numeros enteros
            while (true) {
                int nuevoGen=rand()%(NUM_SERVIDORES);
                if (nuevoGen!=padres[i][j]) padres[i][j]=nuevoGen;break;
            }
        }
        if (!aberracion(padres[i],tiempos,maxCarga))
            poblacion.push_back(padres[i]);
    }
}

int muestramejor(vector<vector<int>> poblacion,
        const vector<int> &tiempos){
    int mejor=0;
    // puede mejorarse ya que esta muestra mejor lo busca en toda la poblaciÃ³n
    // desde luego se puede asumir que es la posiciÃ³n 0 para nuestro caso
    for(int i=0;i<poblacion.size();i++)
        if(calculafitness(poblacion[mejor],tiempos)>calculafitness(poblacion[i],tiempos))
            mejor=i;

    cout << endl<<"La mejor solucion es:" << calculafitness(poblacion[mejor],tiempos)<<endl;
    for(int i=0;i<poblacion[mejor].size();i++)
        cout << poblacion[mejor][i]+1 << "  ";

    vector<int> cargas;
    for (int i=0;i<NUM_SERVIDORES;i++) {
        cargas.push_back(0);
    }
    for (int i=0;i<poblacion[mejor].size();i++) {
        cargas[poblacion[mejor][i]]+=tiempos[i];
    }
    cout<<endl;
    for (int i=0;i<NUM_SERVIDORES;i++) {
        cout<<"Servidor "<<i+1<<": "<<cargas[i]<<endl;
    }


    cout << endl;
    return calculafitness(poblacion[mejor],tiempos);
}

void mochilaAG(const vector<int> &tiempos,int maxCarga) {
    vector<vector<int>> poblacion;
    generapoblacion(poblacion,tiempos,maxCarga);

    for(int i=0;i<ITERACIONES;i++) {
        vector<vector<int>>padres;
        muestrapoblacion(poblacion,tiempos);
        cout << endl;
        seleccion(poblacion,padres,tiempos);
        casamiento(poblacion,padres,tiempos,maxCarga);
        mataclon(poblacion);
        mutacion((poblacion),padres,tiempos,maxCarga);
        inversion(poblacion,padres,tiempos,maxCarga);

        regenerapoblacion(poblacion,tiempos);
        muestrapoblacion(poblacion,tiempos);
        muestramejor(poblacion,tiempos);

    }
}

/*
 * Ejemplo de salida obtenida en una corrida (50 iteraciones):
 * La mejor solucion es:3
 * 2  4  2  1  1  4  3  3
 * Servidor 1: 25
 * Servidor 2: 23
 * Servidor 3: 25
 * Servidor 4: 22
 */
int main() {
    vector<int> tiempos={15,10,8,20,5,12,18,7};
    int maxCarga=25;
    mochilaAG(tiempos,maxCarga);

    return 0;
}