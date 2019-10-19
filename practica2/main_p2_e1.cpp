//*****************************************************************
// File:    main_p2_e1.cpp
// Author:  Diego Marco Beisty 755232
// Date:    octubre 2019
// Coms:    Ejercicio 1, práctica2. Módulo principal
//*****************************************************************
#include <iostream>
#include <thread>
#include "calculos.hpp"

using namespace std;

//***********************************************************
//Proceso calculador
//***********************************************************
void calculador(const Mat A, const Vect x, const int f1, const int f2, Vect&pMV, const int i, bool terminados[]) {
  //Hace calculos
  prod_mat_Vect(A, x, f1, f2, pMV);
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
  cout << "Modulo del vector = " << modulo << endl;
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
    P[i] = thread(&calculador, A, x, i * F, (i * F) + (F - 1), ref(pMV), i, terminados);

  }
  //Lanzo proceso informador
  P[T - 1] = thread(&informador, terminados, ref(pMV), T - 1);
  // Espera activa:  Mientras proceso informador no haya terminado
  while(terminados[T - 1] == false);
  //Recolecto procesos
  for (int i = 0; i < T; i++) {
       P[i].join();
  }
  return 0;
}
