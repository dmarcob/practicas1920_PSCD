//*****************************************************************
// File:   Semaphore_V4.hpp
// Author: PSCD-Unizar
// Date:   octubre 2016
// Coms:   Especificación de una clase de semáforos, extendida, con operaciones
//         "wait(nat)" y "signal(nat)". También se ha extendido con la posibilidad
//         de generar información de "logging"
//         El valor transmitido en la inicialización ha de ser
//         no negativo. Si fuera negativo, da error y aborta la ejecución.
//         La semántica que implementa es la que en
//         M. Ben-Ari, "Principles of Concurrent and Distributed Programming", Addison-Wesley, 2006
//         denomina "Busy-wait" (ver pág. 120)
//         Para los casos en que el uso del semáforo va a ser exclusivamente como "mutex", Cpp
//         suministra la clase "std::mutex", cuya especificación se encuentra en 
//         http://en.cppreference.com/w/cpp/thread/mutex
//*****************************************************************

#ifndef SEMAPHORE_V4_HPP
#define SEMAPHORE_V4_HPP

#include <mutex>
#include <condition_variable>
#include <assert.h>
#include <fstream>

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
//-----------------------------------------------------
using namespace std; //mutex, condition_variable, etc.

class Semaphore {
private:
    mutex mtx;                    //los dos primeros atributos se entenderán al estudiar los monitores
    condition_variable_any cv;
    volatile int count;                    //natural asociado al semáforo  
    volatile bool initialized;             //para manejar constructor genérico: ¿está inicializado?
    //si vamos a querer trabajar con información en un log
    string info;                  //información complementaria para "logging"

public:
    //------------------------- constructores
    //Pre:  n>=0
    //Post: count=n AND initialized
    //Coms: constructor "estándar", tal y como se ha contado en clases de teoría
    //      Ejemplo: "Semaphore s(2);","Semaphore s(2, "mutexVarX");"
    //      Si no se suministra el segundo parámetro, "info" se inicializa a ""
    Semaphore(const int n, const string info = "");

    //Pre:
    //Post: NOT initialized
    //Coms: este constructor es no estándar. Se usará para los casos en que declaremos
    //      un array de semáforos, debiendo posteriormente ejecutar un "setInitiValue"
    //      Un intento de acceso a "wait" o "signal" sin ejecutar "setInitiValue" dará
    //      un error
    //      Ejemplo: "Semaphore s; s.setInitValue(2);" es equivalente a "Semaphore s(2);"
    Semaphore(const string info = "");

    //------------------------- destructor
    //Pre:
    //Post:
    //Coms: se invoca de manera automática al cerrarse el bloque donde ha sido declarado
    ~Semaphore();

    //------------------------- 
    //Pre:  n>=0 AND NOT initialized
    //Post: initialized AND count=n
    //Coms: Semáforos declarados con los constructores "Semaphore()" y "Semaphore(string name)"
    //      requieren que se asigne, mediante este procedimiento, el valor inicial asociado al natural del semáforo
    void setInitValue(const int n, const string info = "");

    //------------------------- operaciones estándar
    //Pre:  initialized
    //Post: <count++>
    //Coms: en el caso de semáforos con información de log, genera un evento en el fichero
    void signal();

    //Pre: initialized
    //Post: <await count>0 
    //          count-- 
    //      >
    //Coms: en el caso de semáforos con información de log, genera un evento en el fichero
    void wait();

    //------------------------- operaciones extendidas
    //Pre:  n>0 AND initialized
    //Post: <count=count+n>
    //Coms: en el caso de semáforos con información de log, genera un evento en el fichero
    void signal(int n);

    //Pre:  n>0 AND initialized
    //Post: <await count>=n 
    //          count = count-n
    //      >
    //Coms: en el caso de semáforos con información de log, genera un evento en el fichero
    void wait(const int n);
};

#endif 
