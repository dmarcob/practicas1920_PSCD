//*****************************************************************
// File:   PuntoAlquiler.hpp
// Author: Diego Marco Beisty 755232
// Date:   noviembre 2019
// Coms:   Fichero interfaz del módulo PuntoAlquiler
//*****************************************************************

#ifndef PUNTO_ALQUILER
#define PUNTO_ALQUILER

#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>

using namespace std;

//-----------------------------------------------------
//  Compilación condicional: ¿quiero hacer "logging"? Modo no-main: var "logger" se declarará en el "main"
#ifdef LOGGING_MODE
    #include <Logger.hpp>
    extern Logger logger;
#else
    #ifndef ADD_EVENT //para evitar redefiniciones
        #define ADD_EVENT(e)  // nada
    #endif
#endif



class PuntoAlquiler {
public:
    //----------------- Constructor
    // Punto de alquiler con <nTandems> tándems y <nBicis> bicicletas individuales
    PuntoAlquiler(const int nTandems, const int nBicis);

    //----------------- Destructor
    ~PuntoAlquiler();

    // Se bloquea hasta que al cliente <id> es le puede conceder una bicicleta individual (en cuyo caso <idPareja>=0)
    // o se le puede conceder un tandem con un compañero, en cuyo caso <idPareja> tendrá su identificador
    void solicitarBicicleta(const int id, int& idPareja);

    // Devuelve la bicicleta o el tándem, de acuerdo con el enunciado
    void devolverBicicleta(const int id, const int idPareja);

private:
    const int NCLIENTES = 20;
    mutex mtxMonitor; //Para la exclusión mutua del monitor
    int bicisDisp, tandemsDisp; // Variables compartidas por los procesos cliente
    int esperando; //Nº de clientes esperando una bici tandem
    int tandemID[2]; //tandemID[0] = id primer cliente esperando en tandem; tandem[1]= id segundo cliente esperando en tandem
    bool barrera[20]; //Si barrera[i] = false -> cliente i no puede devolver tandem, en caso contrario puede devolver tandem
    condition_variable hayBicis; //Se bloquean los clientes cuando no hay bicis (indiv o tandem)
    condition_variable hayCompagnero; //Se bloquan los clientes cuando hay tandem y nadie esperando (espTandem = 0)
    condition_variable poderDevolver[20]; // Se bloquean los clientes cuando al devolver la bici tandem el compañero no ha llegado
};
#endif
