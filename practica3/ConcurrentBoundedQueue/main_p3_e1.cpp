//*****************************************************************
// File:   main_p3_e1.cpp (esqueleto)
// Author: PSCD-Unizar
// Date:   octubre 2018
// Coms:   Parte del práctica 1 de PSCD
//         Compilar mediante el makefile asociado
//         Invocación: main_p3_e1
//*****************************************************************

#include <iostream>
#include <thread>
#include <string>
#include <stdlib.h>
#include <time.h>
#include "ConcurrentBoundedQueue.hpp"

using namespace std;
//-----------------------------------------------------
//  Compilación condicional: ¿quiero hacer "logging"? Modo main: declara la variable "logger"
#ifdef LOGGING_MODE
    #include <Logger.hpp>
    Logger logger("_log_.log");
#else
    #ifndef ADD_EVENT //para evitar redefiniciones
        #define ADD_EVENT(e)  // nada
    #endif
#endif
//-----------------------------------------------------
template <class T>
void insertar(ConcurrentBoundedQueue<T> &cbq, const int nD){
    for(int i=0; i<nD; i++){
    	int num = rand() % 100;
        cbq.enqueue(num);
        this_thread::sleep_for(chrono::milliseconds(num));
    }
}

template <class T>
void extraer(ConcurrentBoundedQueue<T> &cbq, const int nD){
    for(int i=0; i<nD; i++){
    	T v;
        //cbq.firstR(v);
        //cbq.dequeue();
        cbq.first(v);
        this_thread::sleep_for(chrono::milliseconds(rand() % 100));
    }
}

int main(int argc, char* argv[]) {
    const int N = 10; //tamaño de la cola
    const int N_DAT_ESC = 8; //Número de datos a insertar por el escritor
    const int N_DAT_LEC = 6; //Número de datos a insertar por el escritor
    const int N_ESC = 4;  //Número de procesos escritores
    const int N_LEC = 5;  //Número de procesos lectores

    ADD_EVENT("main, BEGIN_FUNC_PROC, 0")
    ConcurrentBoundedQueue<int> cbq(N);
    thread pIns[N_ESC];
    thread pExt[N_LEC];
/*
    for (int i=0; i<N_ESC; i++){
        pIns[i] = thread (&insertar<int>, ref(cbq), N_DAT_ESC);
    }

    for (int i=0; i<N_LEC; i++){
        pExt[i] = thread (&extraer<int>, ref(cbq), N_DAT_LEC);
    }

    for (int i=0; i<N_ESC; i++){
        pIns[i].join();
    }

    for (int i=0; i<N_LEC; i++){
        pExt[i].join();
    }
*/
  pIns[0] = thread (&insertar<int>, ref(cbq), N_DAT_ESC);
  //pIns[1] = thread (&insertar<int>, ref(cbq), N_DAT_ESC);
  pExt[0] = thread (&extraer<int>, ref(cbq), N_DAT_LEC);
  //pExt[1] = thread (&extraer<int>, ref(cbq), N_DAT_LEC);
  pIns[0].join();
  //pIns[1].join();
  pExt[0].join();
  //pExt[1].join();


    cout << "------------------------------------------" << endl;
    cout << "Contenido final de la cola: ";
    cbq.print();
    cout << endl;
    cout << "------------------------------------------" << endl;
    return 0;
    ADD_EVENT("main,END_FUNC_PROC,0");
}
