//*****************************************************************
// File:   ConcurrentBoundedQueue.hpp
// Author: Diego Marco Beisty
// Date:   Octubre 2019
// Coms:   Especificación de una cola FIFO acotada con gestión de acceso concurrente
//*****************************************************************

#ifndef CONCURRENT_BOUNDED_QUEUE
#define CONCURRENT_BOUNDED_QUEUE

#include <Semaphore_V4.hpp>
#include <Logger.hpp>
#include <BoundedQueue.hpp>

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
//-----------------------------------------------------

template <class T>
class ConcurrentBoundedQueue {
public:
    //-----------------------------------------------------
    //Pre:  0<N
    //Post: this->bq=< > AND #this=N
    //Com:  constructor
    ConcurrentBoundedQueue(const int N);

    //-----------------------------------------------------
    //Pre:
    //Post:
    //Com:  destructor
    ~ConcurrentBoundedQueue();

    //-----------------------------------------------------
    //Pre:
    //Post: this->bq=<>
    void empty();

    //-----------------------------------------------------
    //Pre:  this->bq=< d_1 ... d_n >
    //Post: this->bq=< d_1 ... d_n d >
    //Coms: se bloquea hasta que haya un hueco para insertar el dato
    void enqueue(const T d);

    //-----------------------------------------------------
    //Pre:  this->bq=< d_1 ... d_n> and n>0
    //Post: this->bq=< d_2 ... d_n >
    //Coms: se bloquea hasta que this->bq=< d_1 ... d_n> and n>0
    void dequeue();

    //-----------------------------------------------------
    //Pre:  this->bq=< d_1 ... d_n> and n>0
    //Post: f=d_1
    //Coms: se bloquea hasta que this->bq=< d_1 ... d_n> and n>0
    void first(T &f);

    //-----------------------------------------------------
    //Pre:  this->bq=< d_1 ... d_n> and n>0
    //Post: f=d_1 and this->bq=< d_2 ... d_n>
    //Coms: se bloquea hasta que this->bq=< d_1 ... d_n> and n>0
    void firstR(T &f);

    //-----------------------------------------------------
    //Pre:  this->bq=< d_1 ... d_n >
    //Post: l=n
    void length(int &l);

    //-----------------------------------------------------
    //Pre:
    //Post: s=#this->bq
    void size(int &s);

    //-----------------------------------------------------
    //Pre:
    //Post:
    //Com:  muestra la cola por la salida estándar, separando los elementos mediante comas
    void print();

    //-----------------------------------------------------
private:
	int N;
	BoundedQueue<T> *bq;    //"BoundedQueue" con los datos
    //estructura para traducir el diseño mediante <await ...>
	Semaphore mutex;       //Acceso en exclusión mutua a los datos compartidos
	Semaphore b_hay_hueco; //Se bloquean procesos escritores cuando cola llena
	Semaphore b_hay_dato;  //Se bloquean procesos lectores cuando cola vacía
	int d_hay_hueco;       //Nº procesos bloqueados sobre b_hay_hueco
	int d_hay_dato;        //Nº procesos bloqueados sobre b_hay_dato
	void AVISAR();
};
// Para poder trabajar con tipos genéricos, declaración y código deben estar juntos
#include "ConcurrentBoundedQueue.cpp"
#endif
