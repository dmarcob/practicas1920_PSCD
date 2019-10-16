//*****************************************************************
// File:    main_p2_e1.cpp
// Author:  Diego Marco Beisty 755232
// Date:    16-10-2019
// Coms:    Ejercicio 1, práctica2.
//*****************************************************************
#include <iostream>
#include <thread>
#include <stdlib.h>
#include <time.h>
using namespace std;
const int N = 512; //tamaño máximo elementos
const int T= 17; //Número de threads lanzados por el main
typedef float Mat[N][N];
typedef float Vect[N];

                                                                      //TODO: Terminar + comprobar con inicialización de matrices a 2
                                                                      //TODO: Separar en modulos



void pruebas(Vect x, Mat A, Vect pMV) {
  cout << "-------VECTOR x------------" << endl;
  for (int i = 0; i < 10; i++) {
     cout << x [i];
  }
  cout << endl;
    cout << endl <<"------- MATRIZ A------------" << endl;
    for(int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {
        cout << A [i][j] << " ";
      }
      cout << endl;
    }
      cout << endl<< "-------VECTOR pMV------------" << endl;
      for (int i = 0; i < 10; i++) {
         cout << pMV [i] <<" ";
      }
      cout << endl;
}









//************************************************************
//Inicializa un vector con reales pseudoaleatorios
//************************************************************
void inicializarVectorRandom(Vect x) {
    //Inicializo vector con valores [0,100]
    for (int i = 0; i < N; i++) {
       x [i] = (rand() % 100) / 10.0;
    }
}
//************************************************************
//Inicializa los elementos de un vector a cero
//************************************************************
void inicializarVectorCero(Vect x) {
    //Inicializo vector con valores [0,100]
    for (int i = 0; i < N; i++) {
       x [i] = 0;
    }
}

//************************************************************
//Inicializa una matriz de reales con reales pseudoaleatorios
//************************************************************
void inicializarMatrizRandom(Mat A) {
  //Inicializo matriz con valores [0,100]
  for(int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      A [i][j] = (rand() % 1000) / 10.0;
    }
  }
}

//**********************************************************
//Calcula producto matriz x vector
//**********************************************************
void prod_mat_Vect(const Mat A, const Vect x, const int f1, const int f2, Vect&pMV) {
  for (int i = f1; i <= f2; i++) {
    for (int j = 0; j < N; j++) {
      pMV[i] += A[i] [j] * x[j];
    }
  }
}

//**********************************************************
//Devuelve el módulo de un vector
//**********************************************************
float mod_Vect(const Vect x) {
  float resultado = 0;
  for (int i = 0; i < N; i++) {
    resultado += x[i] * x[i];
  }
  return resultado;
}

//***********************************************************
//Proceso calculador
//***********************************************************
void calculador(const Mat A, const Vect x, const int f1, const int f2, Vect&pMV, const int i) {
  cout << "Calculador: "<< i << " " << "filaINI, filaFIN: " << f1 <<f2 << endl;
}

//***********************************************************
//Proceso informador
//***********************************************************
void informador(Vect& x) {
  cout <<"INFORMADOR" << endl;
}


int main() {
  //Declaración variables
  thread P[T];
  Mat A;
  Vect x;
  Vect pMV; // Vector resultado del producto v1 x m
  //Inicialización vector y matriz
  srand(time(NULL));
  inicializarVectorRandom(x);
  inicializarMatrizRandom(A);
  inicializarVectorCero(pMV);
  //Lanzo 16 procesos
  for (int i = 0; i < T - 1; i++) {
    P[i] = thread(&calculador, A, x, i * 16, (i * 16) + 15, ref(pMV), i);

  }
  //Lanzo proceso informador
  //P[i] = PROCESO INFORMADOR

  //Recolecto procesos
  for (int i = 0; i < T - 1; i++) {
       P[i].join();
     }


  return 0;
}
