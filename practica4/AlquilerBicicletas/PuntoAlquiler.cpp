//*****************************************************************
// File:   PuntoAlquiler.cpp
// Author: Diego Marco Beisty 755232
// Date:   noviembre 2019
// Coms:   Fichero implementación del módulo PuntoAlquiler
//*****************************************************************

#include "PuntoAlquiler.hpp"

//-----------------------------------------------------
PuntoAlquiler::PuntoAlquiler(const int nTandems, const int nBicis) {
	cerr << "PuntoAlquiler: " << "BEGIN" << endl;
	this -> bicisDisp = nBicis;
  this -> tandemsDisp = nTandems;
  esperando = 0; //INº clientes esperando una bici tandem inicialemente a cero
  for (int i = 0; i < this -> NCLIENTES; i++) { //Incialmente nadie quiere devolver bici tandem
    barrera[i] = false;
  }
	cerr << "PuntoAlquiler: " << "END" << endl;

}

//-----------------------------------------------------
PuntoAlquiler::~PuntoAlquiler() {
	// ...
}

//-----------------------------------------------------
void PuntoAlquiler::solicitarBicicleta(const int id, int& idPareja) {
	unique_lock<mutex> lck(mtxMonitor);
	cerr << "solicitarBicicleta: " << "BEGIN ... ID: " << id << endl;
	ADD_EVENT("solicitarBicicleta,BEGIN_FUNC_PROC,"+to_string(bicisDisp)+','+to_string(tandemsDisp));
	while (bicisDisp == 0 && (tandemsDisp == 0 || tandemsDisp > 0 && esperando == 2) && !barrera[id]) {
		hayBicis.wait(lck);
	}
	if (bicisDisp > 0) {
		cerr << "ID: " << id << " bicisDisp > 0" << " (" << bicisDisp << ")" << endl;
		bicisDisp--;
		idPareja = 0;
	}
	else { //tandemsDisp > 0
		cerr << "ID: " << id << " bicisDisp = 0" << " (" << bicisDisp << ")" <<" tandemsDisp > 0" << " (" << tandemsDisp << ")" << " esperando (" << esperando << ")" << endl;
			if (esperando == 0) {
					esperando = 1;
					tandemID[0] = id;
					while (esperando == 1 && bicisDisp == 0) {
							cerr << "ID: " << id << " Esperando compagnero de tandem ..." << endl;
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
	cerr << "solicitarBicicleta: " << "END ... ID: " << id <<  " ... IDPAREJA: " << idPareja << "esp: " << esperando << endl;
	ADD_EVENT("solicitarBicicleta,END_FUNC_PROC,"+to_string(bicisDisp)+','+to_string(tandemsDisp));
}




void PuntoAlquiler::devolverBicicleta(const int id, const int idPareja) {						//REVISAR
  unique_lock<mutex> lck(mtxMonitor);
	cerr << "devolverBicicleta: " << "BEGIN ... ID: " << id <<  " ... IDPAREJA: " << idPareja  <<"bicisDisp: (" << bicisDisp <<") tandemsDisp > 0" << " (" << tandemsDisp << ")" << " esperando (" << esperando << ")" << endl;
	ADD_EVENT("devolverBicicleta,BEGIN_FUNC_PROC,"+to_string(bicisDisp)+','+to_string(tandemsDisp));
	if (idPareja == 0) { //Devolver bici individual
		  // cerr << "*********************************************************************************************" << endl;
			// cerr << "ID: " << id << " Devolviendo bici individual ANTES: " << bicisDisp << endl;
			// cerr << "*********************************************************************************************" << endl;
			bicisDisp++;
			hayCompagnero.notify_one();
			hayBicis.notify_all();
			// cerr << "*********************************************************************************************" << endl;
			// cerr << "ID: " << id << " Devolviendo bici individual DESPUES: " << bicisDisp << endl;
			// cerr << "*********************************************************************************************" << endl;


	}
	else {	//Devolver bici tandem
			if (id < idPareja) {
				tandemsDisp++;
				hayBicis.notify_all();
			}
			// barrera[id] = true;
			// if(!barrera[idPareja]){ //Llega el primer cliente de la pareja
			// 	cerr << "----------------------------------------------------------------------------" << endl;
			// 	cerr << "ID: " << id << " Devolviendo bici tandem, PRIMER CLIENTE BLOQUEADO"  << endl;
			// 	cerr << "----------------------------------------------------------------------------" << endl;
			//
			// 		while (!barrera[idPareja]) {
			// 				poderDevolver[id].wait(lck);
			// 		}
			//
			//
			// 		barrera[id] = false;
			// 		barrera[idPareja] = false;
			// 		tandemsDisp++;
			// 		hayBicis.notify_all();
			// 		cerr << "----------------------------------------------------------------------------" << endl;
			// 		cerr << "ID: " << id << " Devolviendo bici tandem, PRIMER CLIENTE DESBLOQUEADO"  << endl;
			// 		cerr << "----------------------------------------------------------------------------" << endl;
			// }
			// else { //Llega el segundo cliente de la pareja
			// 	cerr << "----------------------------------------------------------------------------" << endl;
			// 	cerr << "ID: " << id << " Devolviendo bici tandem, SEGUNDO CLIENTE"  << endl;
			// 	cerr << "----------------------------------------------------------------------------" << endl;
			//
			// 	poderDevolver[idPareja].notify_one();
			// 	poderDevolver[id].notify_one();
			//
			// }
	}
	cerr << "devolverBicicleta: " << "END ... ID: " << id <<  " ... IDPAREJA: " << idPareja  <<"bicisDisp: (" << bicisDisp <<") tandemsDisp > 0" << " (" << tandemsDisp << ")" << " esperando (" << esperando << ")" << endl;
	ADD_EVENT("devolverBicicleta,END_FUNC_PROC,"+to_string(bicisDisp)+','+to_string(tandemsDisp));

}
