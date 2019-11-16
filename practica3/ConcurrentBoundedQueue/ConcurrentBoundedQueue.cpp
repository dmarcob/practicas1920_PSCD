//*****************************************************************
// File:   ConcurrentConcurrentBoundedQueue.cpp
// Author: Diego Marco Beisty
// Date:   Octubre 2019
// Coms:   Especificación de una clase cd cola limitada con gestión de acceso concurrente
//*****************************************************************

#include "ConcurrentBoundedQueue.hpp"
#include <cassert>

//-----------------------------------------------------
template <class T>
ConcurrentBoundedQueue<T>::ConcurrentBoundedQueue(const int N) {
    //semaforos inicializar mutex 1, .. con nombre para el debugging
    bq = new BoundedQueue<T>(N);
    this->N = N;
    this->mutex.setInitValue(1, "mutex");
    this->b_hay_hueco.setInitValue(0, "b_hay_hueco");
    this->b_hay_dato.setInitValue(0, "b_hay_dato");
    this->d_hay_hueco = 0; //0 procesos bloqueados inicialmente sobre b_hay_hueco
    this->d_hay_dato = 0; //0 procesos bloqueados inicialmente sobre b_hay_dato
    empty();
}

//-----------------------------------------------------
template <class T>
ConcurrentBoundedQueue<T>::~ConcurrentBoundedQueue() {
  delete bq;
}

//-----------------------------------------------------
template <class T>
void ConcurrentBoundedQueue<T>::empty() {
    mutex.wait();
    bq->empty();
    AVISAR();
}

//-----------------------------------------------------
template <class T>
void ConcurrentBoundedQueue<T>::enqueue(const T d) {
    mutex.wait();
    if (bq->length() >= N) {
      //Caso cola llena. Me bloqueo sobre b_hay_hueco
      d_hay_hueco = d_hay_hueco + 1;
      mutex.signal();
      b_hay_hueco.wait();
    }
    //Caso colaola no llena, encolo elemento y paso testigo(mutex)
    bq->enqueue(d);
    AVISAR();
}
//-----------------------------------------------------
template <class T>
void ConcurrentBoundedQueue<T>::dequeue() {
    mutex.wait();
    if (bq-> length() <= 0) {
      //Caso cola vacía. Me bloqueo sobre b_hay_dato
      d_hay_dato = d_hay_dato + 1;
      mutex.signal();
      b_hay_dato.wait();
    }
    //Caso cola no vacía, desencolo elemento y paso testigo(mutex)
    bq->dequeue();
    AVISAR();
}
//-----------------------------------------------------
template <class T>
void ConcurrentBoundedQueue<T>::first(T &f) {
  mutex.wait();
  if (bq-> length() <= 0) {
    //Cola vacía
    d_hay_dato = d_hay_dato + 1;
    mutex.signal();
    b_hay_dato.wait();
  }
  //Cola no vacía
  f = bq->first();
  AVISAR();
}
//-----------------------------------------------------
template <class T>
void ConcurrentBoundedQueue<T>::firstR(T &f) {
  mutex.wait();
  if (bq-> length() <= 0) {
    //Cola vacía
    d_hay_dato = d_hay_dato + 1;
    mutex.signal();
    b_hay_dato.wait();                        //COMPLETARR
  }
  //Cola no vacía
  f = bq->first();
  bq->dequeue();
  AVISAR();
}
//-----------------------------------------------------
template <class T>
void ConcurrentBoundedQueue<T>::length(int &l) {
    mutex.wait();
    l = bq -> length();
    AVISAR();
}
//-----------------------------------------------------
template <class T>
void ConcurrentBoundedQueue<T>::size(int &s) {
  mutex.wait();
  s = bq -> size();
  AVISAR();
}
//-----------------------------------------------------
template <class T>
void ConcurrentBoundedQueue<T>::print() {
    mutex.wait();
    bq-> print();
    AVISAR();
}
//-----------------------------------------------------
//-----------------------------------------------------
template <class T>
void ConcurrentBoundedQueue<T>::AVISAR() {
  if (bq->length() > 0 && d_hay_dato > 0) {
    //Caso hay un proceso bloqueado sobre b_hay_dato y
    //la cola no está vacía. Le paso el testigo(mutex)
    d_hay_dato = d_hay_dato - 1;
    b_hay_dato.signal();
  }
  else if (bq->length() < 10 && d_hay_hueco > 0) {
    //Caso hay un proceso bloqueado sobre b_hay_hueco y
    //la cola no está llena. Le paso el testigo(mutex)
    d_hay_hueco = d_hay_hueco - 1;
    b_hay_hueco.signal();
  }
  else {
    //Caso no hay ningún proceso bloqueado. Suelto el testigo(mutex)
    mutex.signal();
  }
}
