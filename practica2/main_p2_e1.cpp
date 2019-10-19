//*****************************************************************
// File:    main_p2_e1.cpp
// Author:  Diego Marco Beisty 755232
// Date:    16-10-2019
// Coms:    Ejercicio 1, práctica2.
//*****************************************************************
#include <iostream>
#include <thread>
#include <stdlib.h> //srand(), rnad()
#include <time.h> //NULL

using namespace std;
const int N = 512; //tamaño máximo elementos
const int T= 17;   //Número de threads lanzados por el main
const int F=32; //Cada thread realiza el producto parcial de <F> filas
typedef float Mat[N][N];
typedef float Vect[N];

                                                                      //TODO: Terminar + comprobar con inicialización de matrices a 2
                                                                      //TODO: Separar en modulos



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
  cerr << "informador: Modulo del vector = " << modulo << endl;
  //Indica que ha terminado
  terminados[i] = true;
}



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
