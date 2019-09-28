//*****************************************************************
// File:    ejercicio_2.cpp
// Author:  Diego Marco Beisty 755232
// Date:    25-9-2019
// Coms:    Ejercicio 2, práctica 1 de PSCD
//*****************************************************************

#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include "stdlib.h"

using namespace std;
//---------------------------------------------------------------------------
//Escribe el mensaje <m> por pantalla y se duerme <retardo> milisegundos.
//Repite este comportamiento <veces> veces.
//---------------------------------------------------------------------------
void saludo(string m, int retardo, int veces) {
    for(int i=1; i<=veces; i++) {
        cout << m+"\n";
        //duerme <retardo> milisegundos
        this_thread::sleep_for(chrono::milliseconds(retardo));
    }
}

//---------------------------------------------------------------------------
//Lanza 10 procesos concurrentes, de manera que el proceso i-ésimo se duerme
//Durante un tiempo aleatorio entre 100 y 300 milisegundos y escribe el mensaje
//correspondiente("Soy 1","Soy 2",...,Soy 10") un número aleatorio de veces,
//entre 5 y 15.
//---------------------------------------------------------------------------
int main() {
    thread P[10];
    string tab ("\t");
    string id;          //String identificador de cada proceso
    srand(time(NULL));  //Semilla de números pseudoaleatorios

    //Lanzo 10 procesos de forma concurrente
    for (int i = 0; i < 10; i++) {
      id = "Soy " + to_string(i + 1); //Identificador para proceso i + 1
      //Proceso P[i] duerme tiempo aleatorio [100,300] ms.
      //Proceso P[i] escribe mensaje <id> numero aleatorio [5,15] veces.
      P[i] = thread(&saludo, tab + id, (rand() % 200) + 100, (rand() % 10) + 5);
      tab += '\t'; //Actualizo <tab>.
    }

    //Espero la terminacion de los 10 procesos
    for (int i = 0; i <10; i++) {
      P[i].join();
    }

    cout << "Fin\n";
    return 0;
}
