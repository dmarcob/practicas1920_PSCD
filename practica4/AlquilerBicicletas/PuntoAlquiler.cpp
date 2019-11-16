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
  esperando = 0; //INº clientes esperando una bici tandem inicialemente a cero
  for (int i = 0; i < this -> NCLIENTES; i++) { //Incialmente nadie quiere devolver bici tandem
    barrera[i] = false;
  }
}

//-----------------------------------------------------
PuntoAlquiler::~PuntoAlquiler() {
	// ...
}

//-----------------------------------------------------
void PuntoAlquiler::solicitarBicicleta(const int id, int& idPareja) {
	unique_lock<mutex> lck(mtxMonitor);
	ADD_EVENT("solicitarBicicleta,BEGIN_FUNC_PROC,"+to_string(bicisDisp)+','+to_string(tandemsDisp));
	while (bicisDisp == 0 && (tandemsDisp == 0 || tandemsDisp > 0 && esperando == 2)) {
		hayBicis.wait(lck);
	}
	if (bicisDisp > 0) {
		bicisDisp--;
		idPareja = 0;
	}
	else if(bicisDisp == 0 && tandemsDisp > 0){ //tandemsDisp > 0
			if (esperando == 0) {
					esperando = 1;
					tandemID[0] = id;
					while (esperando == 1 && bicisDisp == 0) {
							hayCompagnero.wait(lck);
					}
					if (esperando == 1){
							bicisDisp--;
							idPareja = 0;
					}
					else{ //esperando == 2
							tandemsDisp--;
							idPareja = tandemID[1];
					}
					esperando = 0;
			}
	  	else { //esperando == 1
					esperando = 2;
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
	if (idPareja == 0) { //Devolver bici individual
			bicisDisp++;
			hayCompagnero.notify_one();
			hayBicis.notify_all();
	}
	else {	//Devolver bici tandem
			barrera[id - 1] = true;
			if(!barrera[idPareja - 1]){ //Llega el primer cliente de la pareja
					poderDevolver[id - 1].wait(lck);
					barrera[id - 1] = false;
			}
			else { //Llega el segundo cliente de la pareja
				barrera[id - 1] = false;
				tandemsDisp++;
				poderDevolver[idPareja - 1].notify_one();
				hayBicis.notify_all();
			}
	}
	ADD_EVENT("devolverBicicleta,END_FUNC_PROC,"+to_string(bicisDisp)+','+to_string(tandemsDisp));

}
