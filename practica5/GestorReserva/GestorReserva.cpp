//*****************************************************************
// File:   GestorReserva.cpp
// Author: Diego Marco 755232
// Date:   diciembre 2019
// Coms:   Monitor gestor de reservas de un avion. Fichero implementacion
//
//*****************************************************************
#include "GestorReserva.hpp"
#include <stdio.h>
#include <cstring>
GestorReserva::GestorReserva() {
  //Todas las plazas inciialmente libres
  for(int i = 0; i < MAX_PLAZA; i++) {
    for(int j = 0; j < MAX_PLAZA; j++) {
      plazas[i][j] = true;
    }
  }
};

GestorReserva::~GestorReserva() {
  //nothing
};

void GestorReserva::reservar_plaza(bool &reservar, int fila, int columna) {
  unique_lock<mutex> lck(mtxMonitor); //Obtengo el mutex
  reservar = false; //Inicialmente reserva nula
  if(plazas[fila - 1][columna - 1]) { //Consigo reservar
    reservar = true;
    plazas[fila - 1][columna -1] = false;
  }
};

void GestorReserva::plaza_ocupada(char* respuesta, int size, bool& avion_lleno) {
  unique_lock<mutex> lck(mtxMonitor); //Obtengo el mutex
  memset(respuesta, 0, size);
  string mensaje(respuesta);

  avion_lleno = true;
  for(int i = 0; i < MAX_PLAZA; i++) {
    for(int j = 0; j < MAX_PLAZA; j++) {
      if (plazas[i][j]) {avion_lleno = false;}
    }
  }
  if (avion_lleno) {
    mensaje.append("VUELO COMPLETO");
  }
  else {
    mensaje.append("PLAZA OCUPADA:");
    for(int i = 0; i < MAX_PLAZA; i++) {
      for(int j = 0; j < MAX_PLAZA; j++) {
        mensaje.append(plazas[i][j]?"L":"X");
      }
      if(i != MAX_PLAZA - 1) {mensaje.append("-");}
    }
  }
  strcpy(respuesta, mensaje.c_str());
}
