//*****************************************************************
// File:    ejercicio_3.cpp
// Author:  Diego Marco Beisty 755232
// Date:    25-9-2019
// Coms:    Ejercicio3, práctica 1 de PSCD
//*****************************************************************
#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include "stdlib.h"
#include "calculador.hpp"
using namespace std;


int main() {
    thread P[3];
    srand(time(NULL));                //semilla de números  pseudoaleatorios
    double vector[100];
    //Inicializar <vector> con números reales pseudoaleatorios
    for (int i = 0; i < 100; i++) {
      vector[i]=(rand()%1000)/10.0;
    }
    //Creación de objetos
    Calculador uno(vector,100);
    Calculador dos(vector,100);
    Calculador tres(vector,100);
    //Ejecución concurrente de los tres procesos
    P[0] = thread (&Calculador::media, &uno);
    P[1] = thread (&Calculador::maxMin, &dos);
    P[2] = thread (&Calculador::desvTip, &tres);
    //Esperar a que hayan acabado los tres
    for (int i = 0; i < 3; i++) {
      P[i].join();
    }
    //Imprimir resultado
    cout << "#datos:  " << 100 << endl;
    cout << "media:   " << uno.resulMedia << endl;
    cout << "máx:     " << dos.resulMax << endl;
    cout << "mín:     " << dos.resulMin << endl;
    cout << "sigma:   "<< tres.resulDesvTip << endl;

    return 0;
}
