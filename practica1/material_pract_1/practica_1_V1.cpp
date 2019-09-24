//*****************************************************************
// File:   practica_1_V1.cpp
// Author: PSCD-Unizar
// Date:   septiembre 2019
// Coms:   Parte del práctica 1 de PSCD
//         Compilar mediante
//           g++ practica_1_V1.cpp -o practica_1_V1 -std=c++11 -pthread
//*****************************************************************

#include <iostream>
#include <thread>
#include <string>
#include <chrono>

using namespace std;

void saludo(string m, int retardo, int veces) {
    for(int i=1; i<=veces; i++){
        cout << m+"\n";
        this_thread::sleep_for(chrono::milliseconds(retardo));
    }
}

int main() {
    thread th_1(&saludo, "Soy 1 ", 100, 10), //ref. a func, parámetros de func
           th_2(&saludo, "\tSoy 2 ", 150, 15),
           th_3(&saludo, "\t\tSoy 3 ", 300, 5);

    th_1.join();
    th_2.join();
    th_3.join();

    cout << "Fin\n"; 
    return 0;
}