//*****************************************************************
// File:   practica_1_V2.cpp
// Author: PSCD-Unizar
// Date:   septiembre 2019
// Coms:   Parte del práctica 1 de PSCD
//         Compilar mediante
//           g++ practica_1_V2.cpp -o practica_1_V2 -std=c++11 -pthread
//*****************************************************************

#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include "stdlib.h"

using namespace std;

void saludo(string m, int retardo, int veces) {
    for(int i=1; i<=veces; i++) {
        cout << m+"\n";
        this_thread::sleep_for(chrono::milliseconds(retardo)); //duerme <retardo> milisegundos
    }
}

int main() {
    thread P[10];
    string tab ("\t");
    string id;          //mensaje identificador de cada proceso
    srand(time(NULL));  //Semilla pseudoaleatorios

    for (int i = 0; i < 10; i++) { //Lanzo 10 procesos
      id = "Soy " + to_string(i + 1); //Identificador para proceso i + 1
      //Proceso P[i] duerme tiempo aleatorio [100,300] ms.
      //Proceso P[i] escribe mensaje <id> numero aleatorio [5,15] veces.
      P[i] = thread(&saludo, tab + id, (rand() % 200) + 100, (rand() % 10) + 5);
      tab += '\t'; //Actualizo <tab>.
    }

    for (int i = 0; i <10; i++) { //Espero terminacion 10 procesos
      P[i].join();
    }

    cout << "Fin\n";
    return 0;
}
