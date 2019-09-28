//*****************************************************************
// File:    calculador.cpp
// Author:  Diego Marco Beisty 755232
// Date:    25-9-2019
// Coms:    Fichero implementación del módulo "calculador"(ejercicio3)
//*****************************************************************
#include <cmath>
#include <iostream>
#include "calculador.hpp"

using namespace std;

//--------------------------------------------------------
//Constructor, recibe vector de reales pseudoaleatorios y
//el número de elementos a tratar
//--------------------------------------------------------
Calculador::Calculador (double vector[], int numElementos) {
    this->vector = vector;
    this->numElementos = numElementos;
};

//---------------------------------------------------------
//Operación media aritmética
//---------------------------------------------------------
void Calculador::media() {
    double suma = 0;
    for (int i = 0; i < numElementos; i++) {
        suma += vector[i];
    }
    this->resulMedia = suma / numElementos;
};

//---------------------------------------------------------
//Operación máximo y mínimo
//---------------------------------------------------------
void Calculador::maxMin() {
    double elemento = vector[0];
    for (int i = 1; i < numElementos; i++) {
      if (vector[i] < elemento) {elemento = vector[i];}
    }
    this-> resulMin = elemento;

    elemento = vector[0];
    for (int i = 1; i < numElementos; i++) {
      if (vector[i] > elemento) {elemento = vector[i];}
    }
    this -> resulMax = elemento;
};

//---------------------------------------------------------
//Operación desviación típica
//---------------------------------------------------------
void Calculador::desvTip() {
    int suma = 0;
        double varianza = 0;
        for (int i=0; i<numElementos; i++) {
             suma += vector[i];
        }
        suma = suma / 100;
        for (int i=0; i<100; i++) {
             varianza += pow((vector[i] - suma ), 2.0);
        }

        this->resulDesvTip = sqrt(varianza / 99);
};
