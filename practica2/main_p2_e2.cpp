//*****************************************************************
// File:    main_p2_e2.cpp
// Author:  Diego Marco Beisty 755232
// Date:    1octubre 2019
// Coms:    Ejercicio 2, práctica2. Módulo principal
//*****************************************************************
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include "calculos.hpp"

using namespace std;

//***********************************************************
//Proceso calculador
//***********************************************************
void calculador(const Mat A, const Vect x, const int f1, const int f2, Vect&pMV, const int i, bool terminados[],
                chrono::nanoseconds& tMax, atomic_flag& tas, int& registro,thread::id& idMasLento) {
  //Inicia contador
  chrono::steady_clock::time_point start = chrono::steady_clock::now();
  //Hace calculos
  prod_mat_Vect(A, x, f1, f2, pMV);
  //Finaliza contador
  chrono::steady_clock::time_point end = chrono::steady_clock::now();
  chrono::nanoseconds t = chrono::duration_cast<chrono::nanoseconds>(end - start);
  while(tas.test_and_set()); //Pre-protocolo. (Espera activa con instrucción test and set)
  //Sección Crítica
  cout << "Tiempo de ejecución (nsegs): " << t.count() << endl;
  if (registro == 0) {
    //Caso es el primer thread que entra a la S.C
    tMax = t;
    idMasLento = this_thread::get_id();
    registro++; //Actualizo registro de procesos que han pasado por la S.C
  }
  else {
    //Caso NO es el primer thread que entra a la S.C
    if (t > tMax) {
      //Si es más lento, actualiza valores
      tMax = t;
      idMasLento = this_thread::get_id();
    }
    registro++; //Actualizo registro de procesos que han pasado por la S.C
  }
  tas.clear(); //Post-protocolo
  //Indica que ha terminado
  terminados[i] = true;
}

//***********************************************************
//Proceso informador
//***********************************************************
void informador(bool terminados[], Vect pMV, int i) {
  //Espera activa: Mientras los 16 procesos calculador no hayan terminado
  for (int i = 0; i < T - 1; i++) {
    while(terminados[i] == false);
  }
  //Hace calculos
  float modulo = mod_Vect(pMV);
  //Muestra el resultado por pantalla
  cerr << "informador: Modulo del vector = " << modulo << endl;
  //Indica que ha terminado
  terminados[i] = true;
}

//***********************************************************
//Proceso main
//***********************************************************
int main() {
  //Declaración variables
  thread P[T];
  Mat A;
  Vect x;
  Vect pMV; // Vector resultado del producto <A> x <x>

  //--------------EJERCICIO 2
  atomic_flag tas = ATOMIC_FLAG_INIT;
  chrono::nanoseconds tMax;
  thread::id idMasLento;
  int registro = 0; //Número de procesos que han accedido a la S.C
  //-------------------------
  //Declaración variables de sincronización
  bool terminados[T]; //[terminados[0],terminados[T-2]] indican el estado de los 16 procesos calculador
                      //terminados[T-1] indica el estado del proceso informador
                      //terminados[i] == false, indica que el thread <i> no ha terminado.
                      //terminados[i] == true, indica que el thread <i> ya ha terminado.
  //Inicialización vectores y matriz
  srand(time(NULL));
  inicializarVectorRandom(x);
  inicializarMatrizRandom(A);
  inicializarVectorCero(pMV);
  inicializarVectorFalse(terminados);
  //Lanzo 16 procesos calculador
  for (int i = 0; i < T - 1; i++) {
    //Cada proceso calcula el producto parcial de 32 filas.
    P[i] = thread(&calculador, A, x, i * F, (i * F) + (F - 1), ref(pMV), i, terminados,
                  ref(tMax), ref(tas),ref(registro), ref(idMasLento));

  }
  //Lanzo proceso informador
  P[T - 1] = thread(&informador, terminados, ref(pMV), T - 1);
  // Espera activa:  Mientras proceso informador no haya terminado
  while(terminados[T - 1] == false);
  //Recolecto procesos
  for (int i = 0; i < T; i++) {
       P[i].join();
  }
  //Muestra las métricas del thread más lento
  cout <<" ___________________" << endl;
  cout <<"|tMax: " << tMax.count() << endl;
  cout <<"|idMasLento: " << idMasLento << endl;
  cout <<"|___________________" << endl;
  return 0;
}
