//*****************************************************************
// File:   main_p4.cpp
// Author: Diego Marco Beisty 755232
// Date:   noviembre 2019
// Coms:   Fichero con el programa principal de la práctica 4
//*****************************************************************

#include <iostream>
#include <thread>
#include <string>
#include "PuntoAlquiler.hpp"

using namespace std;
//-----------------------------------------------------
//  Compilación condicional: ¿quiero hacer "logging"?
#ifdef LOGGING_MODE
    #include <Logger.hpp>
    Logger logger("_log_.log");
#else
    #ifndef ADD_EVENT //para evitar redefiniciones
        #define ADD_EVENT(e)  // nada
    #endif
#endif
//-----------------------------------------------------

//Duerme al thread que le invoca entre [0, 100] milisegundos
void dormir(){
  int num = rand() % 100;
  this_thread::sleep_for(chrono::milliseconds(num));
}

//Simula el comportamiento de un cliente que durante n_iter veces solicita una bicicleta,
//una vez ha logrado su alquiler, da un paseo y devuelve la bicicleta
void cliente (PuntoAlquiler &alquiler, const int id, const int n_iter) {
  int idPareja;
  for (int i = 0; i < n_iter; i ++) {
    dormir();
    alquiler.solicitarBicicleta(id, idPareja);
    dormir();
  //  cerr << "ID = " << id << " IDPAREJA: " << idPareja << "i: " << i<< endl;
    alquiler.devolverBicicleta(id, idPareja);
  }
}

int main(int argc, char* argv[]) {
    const int NTANDEMS = 3;  //num. de tandems disponibles
    const int NBICIS = 5; //num. de bicis individuales disponibles
    const int NCLIENTES = 20; //num, de clientes
    const int NITER = 5; //num. de iteraciones por cliente
    thread pCli[NCLIENTES];
    ADD_EVENT("main,BEGIN_FUNC_PROC,"+to_string(NBICIS)+','+to_string(NTANDEMS));
    PuntoAlquiler alquiler(NTANDEMS, NBICIS);

    for (int i=0; i<NCLIENTES; i++){
        pCli[i] = thread (&cliente, ref(alquiler), i + 1, NITER);
    }
    for (int i=0; i<NCLIENTES; i++){
        pCli[i].join();
    }


    ADD_EVENT("main,END_FUNC_PROC,"+to_string(NBICIS)+','+to_string(NTANDEMS));
    return 0;
}
