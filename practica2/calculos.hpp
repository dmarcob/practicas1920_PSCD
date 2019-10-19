//*****************************************************************
// File:    calculos.cpp
// Author:  Diego Marco Beisty 755232
// Date:    octubre 2019
// Coms:    Fichero interfaz del módulo calculos
//*****************************************************************
#ifndef CALCULOS_H
#define CALCULOS_H

#include <iostream>
#include <stdlib.h> //srand(), rnad()
#include <time.h> //NULL

const int N = 512; //tamaño máximo elementos
const int T= 17;   //Número de threads lanzados por el main
const int F=32; //Cada thread realiza el producto parcial de <F> filas
typedef float Mat[N][N];
typedef float Vect[N];

using namespace std;

//************************************************************
//Inicializa un vector con reales pseudoaleatorios
//************************************************************
void inicializarVectorRandom(Vect x);

//************************************************************
//Inicializa los elementos de un vector a cero
//************************************************************
void inicializarVectorCero(Vect x);

//************************************************************
//Inicializa una matriz de reales con reales pseudoaleatorios
//************************************************************
void inicializarMatrizRandom(Mat A);

//************************************************************
//Inicializa un vector de booleanos a false
//************************************************************
void inicializarVectorFalse(bool terminados[]);

//**********************************************************
//Calcula producto matriz x vector
//**********************************************************
void prod_mat_Vect(const Mat A, const Vect x, const int f1, const int f2, Vect&pMV);

//**********************************************************
//Devuelve el módulo de un vector
//**********************************************************
float mod_Vect(const Vect x);

//***********************************************************
//Función para hacer pruebas
//**********************************************************
void pruebas(const Vect x,const Mat A,const Vect pMV, bool terminados[]);

#endif
