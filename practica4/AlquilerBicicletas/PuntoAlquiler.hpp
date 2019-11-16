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
    int solicitandoTand; //num. clientes solicitando una bici tandem. Posibles valores:
                         //solicitandoTand == 0 : Ningún cliente solicitando una bici tandem.
                         //solicitandoTand == 1 : Un cliente solicitando una bici tandem
                         //solicitandoTand == 2 : Dos procesos solicitando una bici tandem
    int tandemID[2];  //Identificadores de los clientes que solicitan una misma bici tandem
                      //tandemID[0] = id primer cliente que solicita bici tandem; tandem[1]= id segundo cliente que solicita bici tandem
    bool barrera[20]; //Indica si el cliente i: 1..20 ha llegado al punto de alquiler a devolver una bici tandem
    condition_variable hayBicis; //Se bloquean los clientes cuando no hay ni bicis individuales ni bici tandem.
    condition_variable hayCompagnero; //Se bloquean los clientes cuando son los primeros en solicitar una bici tandem
    condition_variable poderDevolver[20]; // Se bloquean los clientes sii al devolver una bici tandem, su compañero todavía no ha llegado a devolverla
};
#endif
