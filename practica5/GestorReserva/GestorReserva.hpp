//*****************************************************************
// File:   GestorReserva.hpp
// Author: Diego Marco 755232
// Date:   diciembre 2019
// Coms:   Monitor gestor de reservas de un avion. Fichero interfaz
//
//*****************************************************************

#ifndef GESTOR_RESERVA
#define GESTOR_RESERVA

#include <string>
#include <mutex>
#include <condition_variable>

using namespace std;

const int MAX_PLAZA = 4;
class GestorReserva {
public:
  //Constructor
  GestorReserva();

  //Destructor
  ~GestorReserva();

  //Si ha conseguido reservar la plaza de avion identificada
  //por "fila" y "columna", entonces  "reservar" vale true.
  //En caso contrario "reservar" vale false.
  void reservar_plaza(bool &reservar, int fila, int columna);

  //Devuelve en respuesta la respuesta del servidor ServidorMulticliente
  //cuando no se ha conseguido reservar plaza y quedan plazas libres ó
  //cuando no se ha conseguido reservar plaza y el avion esta lleno
  void plaza_ocupada(char* respuesta, int size, bool& avion_lleno);

private:
  bool plazas[MAX_PLAZA][MAX_PLAZA]; //Matriz de asientos de avion 
  mutex mtxMonitor;

};
#endif
