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
    //--------------------------------------------------------
    //Constructor, recibe vector de reales pseudoaleatorios y
    //el número de elementos a tratar
    //--------------------------------------------------------
    Calculador(double vector[], int numElementos);

    //---------------------------------------------------------
    //Operación media aritmética
    //---------------------------------------------------------
    void media();

    //---------------------------------------------------------
    //Operación máximo y mínimo
    //---------------------------------------------------------
    void maxMin();

    //---------------------------------------------------------
    //Operación desviación típica
    //---------------------------------------------------------
    void desvTip();

    double* vector; //Puntero a vector de reales aleatorios
    int numElementos;//Número de elementos del vector
    //Resultados de ejecutar cada operación
    double resulMedia, resulMin, resulMax, resulDesvTip;
};
#endif
