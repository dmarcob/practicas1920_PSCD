//*****************************************************************
// File:    calculos.cpp
// Author:  Diego Marco Beisty 755232
// Date:    19-10-2019
// Coms:    Fichero implementación del módulo calculos
//*****************************************************************

#include "calculos.hpp"

//************************************************************
//Inicializa un vector con reales pseudoaleatorios
//************************************************************
void inicializarVectorRandom(Vect x) {
    //Inicializo vector con valores [0,10]
    for (int i = 0; i < N; i++) {
       x [i] = (rand() % 100) / 10.0;
    }
}
//************************************************************
//Inicializa los elementos de un vector a cero
//************************************************************
void inicializarVectorCero(Vect x) {
    //Inicializo vector a 0
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

//************************************************************
//Inicializa un vector de booleanos a false
//************************************************************
void inicializarVectorFalse(bool terminados[]) {
  for (int i = 0; i < T; i++) {
     terminados[i] = false;
  }
}

//**********************************************************
//Calcula producto matriz x vector
//**********************************************************
void prod_mat_Vect(const Mat A, const Vect x, const int f1, const int f2, Vect&pMV) {
  for (int i = f1; i <= f2; i++) {
    for (int j = 0; j < N; j++) {
      pMV[i] += A[i][j] * x[j];
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
//Función para hacer pruebas
//**********************************************************
void pruebas(const Vect x,const Mat A,const Vect pMV, bool terminados[]) {
    cout << "-------VECTOR x------------" << endl;
    for (int i = 0; i < 10; i++) {
        cout << x [i]<<" ";
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
    cout << endl<< "-------VECTOR terminados------------" << endl;
    for (int i = 0; i < T; i++) {
        cout<< terminados[i] << " ";
    }
}
