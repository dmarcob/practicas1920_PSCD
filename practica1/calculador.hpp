//*****************************************************************
// File:    calculador.h
// Author:  Diego Marco Beisty 755232
// Date:    25-9-2019
// Coms:    Fichero interfaz del módulo "calculador"(ejercicio3)
//*****************************************************************
#ifndef CALCULADOR_H
#define CALCULADOR_H

class Calculador {
public:
    //Constructor, recibe vector de reales pseudoaleatorios y
    //el número de elementos a tratar
    Calculador(double vector[], int numElementos);
    //Operación media aritmética sobre vector de números reales pseudoaleatorios
    void media();
    //Calcula el elemento máximo y mínimo sobre los elementos de un vector
    void maxMin();
    //Calcula la desviación típica sobre los elementos de un vector
    void desvTip();
    double* vector;
    int numElementos; 
    double resulMedia, resulMin, resulMax, resulDesvTip;
};
#endif
