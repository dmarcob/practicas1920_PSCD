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
        cout <<"queue: cola #########LLENA########" << endl;
      //Cola llena
      d_hay_hueco = d_hay_hueco + 1;
      mutex.signal();
      b_hay_hueco.wait();
    }
      cout <<"queue: cola no llena" << endl;
    //Cola no llena
    bq->enqueue(d);
    AVISAR();
}
//-----------------------------------------------------
template <class T>
void ConcurrentBoundedQueue<T>::dequeue() {
    mutex.wait();
    if (bq-> length() <= 0) {
      cout <<"dequeue: cola @@@@@@@@@@@VACIA@@@@@@@@@@" << endl;
      //Cola vacía
      d_hay_dato = d_hay_dato + 1;
      mutex.signal();
      b_hay_dato.wait();
    }
      cout <<"dequeue: cola no vacia" << endl;
    //Cola no vacía
    bq->dequeue();
    AVISAR();
}
//-----------------------------------------------------
template <class T>
void ConcurrentBoundedQueue<T>::first(T &f) {
  mutex.wait();
  if (bq-> length() <= 0) {
    cout <<"first: cola @@@@@@@@@@@VACIA@@@@@@@@@@" << endl;
    //Cola vacía
    d_hay_dato = d_hay_dato + 1;
    mutex.signal();
    b_hay_dato.wait();
  }
    cout <<"first: cola no vacia" << endl;
  //Cola no vacía
  f = bq->first(); cout << "first: leido= " << f << endl;
  AVISAR();
}
//-----------------------------------------------------
template <class T>
void ConcurrentBoundedQueue<T>::firstR(T &f) {
  mutex.wait();
  if (bq-> length() <= 0) {
    cout <<"firstR: cola @@@@@@@@@@@VACIA@@@@@@@@@@" << endl;
    //Cola vacía
    d_hay_dato = d_hay_dato + 1;
    mutex.signal();
    b_hay_dato.wait();                        //COMPLETARR
  }
    cout <<"firstR: cola no vacia" << endl;
  //Cola no vacía
  f = bq->first(); cout << "firstR: leido= " << f << endl;
  bq->dequeue();
  AVISAR();
}
//-----------------------------------------------------
template <class T>
void ConcurrentBoundedQueue<T>::length(int &l) {
    mutex.wait();
    cout <<"length: " << bq -> length() << endl;
    l = bq -> length();
    AVISAR();
}
//-----------------------------------------------------
template <class T>
void ConcurrentBoundedQueue<T>::size(int &s) {
  mutex.wait();
  cout <<"size: " << bq -> size() << endl;
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
  cout << "avisar:COLA---->" << endl;
  bq-> print(); cout << endl;
  cout << "--------------" << endl;
  if (bq->length() > 0 && d_hay_dato > 0) {
    d_hay_dato = d_hay_dato - 1;
    b_hay_dato.signal();
  cout <<"avisar: [d_hay_dato= " << d_hay_dato <<"], paso testigo a -----> lector" <<endl;}
  else if (bq->length() < 10 && d_hay_hueco > 0) {
    d_hay_hueco = d_hay_hueco - 1;
    b_hay_hueco.signal();
  cout <<"avisar: [d_hay_huevo= " << d_hay_hueco <<"], paso testigo a ----> escritor" <<endl;}
  else {mutex.signal(); cout << "avisar: suelto el mutex" << endl;}
}
