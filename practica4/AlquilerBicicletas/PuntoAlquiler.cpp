//*****************************************************************
// File:   PuntoAlquiler.cpp
// Author: Diego Marco Beisty 755232
// Date:   noviembre 2019
// Coms:   Fichero implementación del módulo PuntoAlquiler
//*****************************************************************

#include "PuntoAlquiler.hpp"

//-----------------------------------------------------
PuntoAlquiler::PuntoAlquiler(const int nTandems, const int nBicis) {
	this -> bicisDisp = nBicis;
  this -> tandemsDisp = nTandems;
  solicitandoTand = 0; //Ningún cliente solicita una bici tandem
  for (int i = 0; i < this -> NCLIENTES; i++) {
    barrera[i] = false; // Cliente i no ha llegado al punto de alquiler a devolver una bici tandem
  }
}

//-----------------------------------------------------
PuntoAlquiler::~PuntoAlquiler() {
}

//-----------------------------------------------------
void PuntoAlquiler::solicitarBicicleta(const int id, int& idPareja) {
	unique_lock<mutex> lck(mtxMonitor);
	ADD_EVENT("solicitarBicicleta,BEGIN_FUNC_PROC,"+to_string(bicisDisp)+','+to_string(tandemsDisp));
	while (bicisDisp == 0 && (tandemsDisp == 0 || tandemsDisp > 0 && solicitandoTand == 2)) {
		//Cliente se bloquea si no hay bicis disponibles.
		//Como no se cumple E < S < W, el cliente también se bloquea si no hay bicis individuales y hay dos clientes solicitando una bici tandem
		hayBicis.wait(lck);
	}
	if (bicisDisp > 0) {
		//Caso bici individual disponible
		bicisDisp--;
		idPareja = 0;
	}
	else{
		//Caso bici tandem disponible
		if (solicitandoTand == 0) {
				//Caso ningún cliente solicitando una bici tandem
				solicitandoTand = 1;
				tandemID[0] = id;
				while (bicisDisp == 0 && solicitandoTand == 1) {
						//Cliente se bloquea mientras no llegue una bici individual o no llegue un segundo cliente
						hayCompagnero.wait(lck);
				}
				if (solicitandoTand == 1){
						//Antes de llegar un segundo cliente una bici individual es devuelta
						bicisDisp--;
						idPareja = 0;
				}
				else{
						//Ha llegado un segundo cliente
						tandemsDisp--;
						idPareja = tandemID[1];
				}
				solicitandoTand = 0;
				hayBicis.notify_all();
		}
	 	else {
				//Caso hay un cliente solicitando una bici tandem
				solicitandoTand = 2;
				tandemID[1] = id;
				idPareja = tandemID[0];
				hayCompagnero.notify_one();
		}
	}
	ADD_EVENT("solicitarBicicleta,END_FUNC_PROC,"+to_string(bicisDisp)+','+to_string(tandemsDisp));
}

void PuntoAlquiler::devolverBicicleta(const int id, const int idPareja) {
  unique_lock<mutex> lck(mtxMonitor);
	ADD_EVENT("devolverBicicleta,BEGIN_FUNC_PROC,"+to_string(bicisDisp)+','+to_string(tandemsDisp));
	if (idPareja == 0) {
		  //Cliente devuelve una bici individual
			bicisDisp++;
			hayCompagnero.notify_one();
			hayBicis.notify_all();
	}
	else {
			//Cliente devuelve una bici tandem
			barrera[id - 1] = true; //Cliente ha llegado al punto de alquiler para devolver la bici tandem
			if(!barrera[idPareja - 1]){
					//Caso la pareja no ha llegado al punto de alquiler para devolver la bici tandem
					poderDevolver[id - 1].wait(lck);
					barrera[id - 1] = false;
			}
			else {
				//Caso la pareja ha llegado al punto de alquiler
				barrera[id - 1] = false;
				tandemsDisp++;
				poderDevolver[idPareja - 1].notify_one();
				hayBicis.notify_all();
			}
	}
	ADD_EVENT("devolverBicicleta,END_FUNC_PROC,"+to_string(bicisDisp)+','+to_string(tandemsDisp));
}
