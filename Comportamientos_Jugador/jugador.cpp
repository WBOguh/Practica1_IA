#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;

Action ComportamientoJugador::think(Sensores sensores)
{

	Action accion = actIDLE;
	int a;

	// Mostrar el valor de los sensores
	cout << "Posicion: fila " << sensores.posF << " columna " << sensores.posC;
	switch (sensores.sentido)
	{
		case norte:	  cout << " Norte\n";	break;
		case noreste: cout << " Noreste\n";	break;
		case este:    cout << " Este\n";	break;
		case sureste: cout << " Sureste\n";	break;
		case sur:     cout << " Sur\n";	break;
		case suroeste:cout << " Suroeste\n";	break;
		case oeste:   cout << " Oeste\n";	break;
		case noroeste:cout << " Noroeste\n";	break;
	}
	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];

	cout << "  Agentes: ";
	for (int i=0; i<sensores.agentes.size(); i++)
		cout << sensores.agentes[i];

	cout << "\nColision: " << sensores.colision;
	cout << "  Reset: " << sensores.reset;
	cout << "  Vida: " << sensores.vida << endl<< endl;
	
	switch (last_action)
	{
		case actWALK:
			switch (current_state.brujula)
			{
				case norte: current_state.fil--; break;
				case sur: current_state.fil++; break;
				case este: current_state.col++; break;
				case oeste: current_state.col--; break;
				case noreste: current_state.fil--; current_state.col++; break;
				case noroeste: current_state.fil--; current_state.col--; break;
				case sureste: current_state.fil++; current_state.col++; break;
				case suroeste: current_state.fil++; current_state.col--; break;
			}
			break;
		case actRUN:
			switch (current_state.brujula)
			{
				case norte: current_state.fil-=2; break;
				case sur: current_state.fil+=2; break;
				case este: current_state.col+=2; break;
				case oeste: current_state.col-=2; break;
				case noreste: current_state.fil-=2; current_state.col+=2; break;
				case noroeste: current_state.fil-=2; current_state.col-=2; break;
				case sureste: current_state.fil+=2; current_state.col+=2; break;
				case suroeste: current_state.fil+=2; current_state.col-=2; break;
			}
			break;
		case actTURN_SR:
			a = current_state.brujula;
			a = (a + 1) % 8;
			current_state.brujula = static_cast<Orientacion>(a);
			break;
		case actTURN_L:
			a = current_state.brujula;
			a = (a + 6) % 8;
			current_state.brujula = static_cast<Orientacion>(a);
			break;
	}
if (sensores.terreno[0]=='G' and !bien_situado){
	current_state.fil = sensores.posF;
	current_state.col= sensores.posC;
	current_state.brujula = sensores.sentido;
	bien_situado = true;
}
if (bien_situado){
	PonerTerrenoMatriz(sensores.terreno, current_state, mapaResultado, current_state.brujula, sensores);
	}
if (sensores.terreno[2] == 'T' or sensores.terreno[2] == 'S' or sensores.terreno[2] == 'G' and sensores.agentes[2] == '_'){
	accion = actWALK;
	}else if(!girar_derecha){
		accion = actTURN_L;
		girar_derecha = (rand() % 2 == 0);
	}else{
		accion = actTURN_SR;
		girar_derecha = (rand() % 2 == 0);
	}

last_action = accion;	
	return accion;
}
void ComportamientoJugador::PonerTerrenoMatriz(const vector<unsigned char> &terreno, const state &st, vector <vector< unsigned char> > &matriz, Orientacion pos, Sensores sensor){
    if (st.fil < 0 || st.fil >= matriz.size() || st.col < 0 || st.col >= matriz[0].size()) {
        cout << "Error: Índices fuera de los límites de la matriz." << endl;
        return;
    }
    matriz[st.fil][st.col] = terreno[0];
	if (sensor.nivel != 3){
		switch (st.brujula){
				case norte:
					matriz[st.fil - 1][st.col - 1] = sensor.terreno[1]; 
					matriz[st.fil - 1][st.col] = sensor.terreno[2];
					matriz[st.fil - 1][st.col + 1] = sensor.terreno[3];
					matriz[st.fil - 2][st.col - 2] = sensor.terreno[4];
					matriz[st.fil - 2][st.col - 1] = sensor.terreno[5];
					matriz[st.fil - 2][st.col] = sensor.terreno[6];
					matriz[st.fil - 2][st.col + 1] = sensor.terreno[7];
					matriz[current_state.fil - 2][current_state.col + 2] = sensor.terreno[8];
					matriz[st.fil - 3][st.col - 3] = sensor.terreno[9];
					matriz[st.fil - 3][st.col - 2] = sensor.terreno[10];
					matriz[st.fil - 3][st.col - 1] = sensor.terreno[11];
					matriz[st.fil - 3][st.col] = sensor.terreno[12];
					matriz[st.fil - 3][st.col + 1] = sensor.terreno[13];
					matriz[st.fil - 3][st.col + 2] = sensor.terreno[14];
					matriz[st.fil - 3][st.col + 3] = sensor.terreno[15];
					break;

				case noreste:
					matriz[st.fil - 1][st.col] = sensor.terreno[1];
					matriz[st.fil - 1][st.col + 1] = sensor.terreno[2];
					matriz[st.fil][st.col + 1] = sensor.terreno[3];
					matriz[st.fil - 2][st.col] = sensor.terreno[4];
					matriz[st.fil - 1][st.col + 1] = sensor.terreno[5];
					matriz[st.fil - 2][st.col + 2] = sensor.terreno[6];
					matriz[st.fil - 1][st.col + 2] = sensor.terreno[7];
					matriz[st.fil][st.col + 2] = sensor.terreno[8];
					matriz[st.fil - 3][st.col] = sensor.terreno[9];
					matriz[st.fil - 3][st.col + 1] = sensor.terreno[10];
					matriz[st.fil - 3][st.col + 2] = sensor.terreno[11];
					matriz[st.fil - 3][st.col + 3] = sensor.terreno[12];
					matriz[st.fil - 2][st.col + 3] = sensor.terreno[13];
					matriz[st.fil - 2][st.col + 3] = sensor.terreno[14];
					matriz[st.fil][st.col + 3] = sensor.terreno[15];
					break;

				case este:
					matriz[st.fil - 1][st.col + 1] = sensor.terreno[1];
					matriz[st.fil][st.col + 1] = sensor.terreno[2];
					matriz[st.fil + 1][st.col + 1] = sensor.terreno[3];
					matriz[st.fil - 2][st.col + 2] = sensor.terreno[4];
					matriz[st.fil - 1][st.col + 2] = sensor.terreno[5];
					matriz[st.fil][st.col + 2] = sensor.terreno[6];
					matriz[st.fil + 1][st.col + 2] = sensor.terreno[7];
					matriz[st.fil + 2][st.col + 2] = sensor.terreno[8];
					matriz[st.fil - 3][st.col + 3] = sensor.terreno[9];
					matriz[st.fil - 2][st.col + 3] = sensor.terreno[10];
					matriz[st.fil - 1][st.col + 3] = sensor.terreno[11];
					matriz[st.fil][st.col + 3] = sensor.terreno[12];
					matriz[st.fil + 1][st.col + 3] = sensor.terreno[13];
					matriz[st.fil + 2][st.col + 3] = sensor.terreno[14];
					matriz[st.fil + 3][st.col + 3] = sensor.terreno[15];
					break;

				case sureste:
					matriz[st.fil][st.col + 1] = sensor.terreno[1];
					matriz[st.fil + 1][st.col + 1] = sensor.terreno[2];
					matriz[st.fil+ 1][st.col] = sensor.terreno[3];
					matriz[st.fil][st.col + 2] = sensor.terreno[4];
					matriz[st.fil + 1][st.col + 2] = sensor.terreno[5];
					matriz[st.fil + 2][st.col + 2] = sensor.terreno[6];
					matriz[st.fil + 2][st.col + 1] = sensor.terreno[7];
					matriz[st.fil + 2][st.col] = sensor.terreno[8];
					matriz[st.fil][st.col + 3] = sensor.terreno[9];
					matriz[st.fil + 1][st.col + 3] = sensor.terreno[10];
					matriz[st.fil + 2][st.col + 3] = sensor.terreno[11];
					matriz[st.fil + 3][st.col + 3] = sensor.terreno[12];
					matriz[st.fil + 3][st.col + 2] = sensor.terreno[13];
					matriz[st.fil + 3][st.col + 1] = sensor.terreno[14];
					matriz[st.fil + 3][st.col] = sensor.terreno[15];
					break;

				case sur: 
					matriz[st.fil + 1][st.col + 1] = sensor.terreno[1];
					matriz[st.fil + 1][st.col] = sensor.terreno[2];
					matriz[st.fil + 1][st.col - 1] = sensor.terreno[3];
					matriz[st.fil + 2][st.col + 2] = sensor.terreno[4];
					matriz[st.fil + 2][st.col + 1] = sensor.terreno[5];
					matriz[st.fil + 2][st.col] = sensor.terreno[6];
					matriz[st.fil + 2][st.col - 1] = sensor.terreno[7];
					matriz[st.fil + 2][st.col - 2] = sensor.terreno[8];
					matriz[st.fil + 3][st.col + 3] = sensor.terreno[9];
					matriz[st.fil + 3][st.col + 2] = sensor.terreno[10];
					matriz[st.fil + 3][st.col + 1] = sensor.terreno[11];
					matriz[st.fil + 3][st.col] = sensor.terreno[12];
					matriz[st.fil + 3][st.col - 1] = sensor.terreno[13];
					matriz[st.fil + 3][st.col - 2] = sensor.terreno[14];
					matriz[st.fil + 3][st.col - 3] = sensor.terreno[15];
					break;

				case suroeste:
					matriz[st.fil + 1][st.col] = sensor.terreno[1];
					matriz[st.fil + 1][st.col - 1] = sensor.terreno[2];
					matriz[st.fil][st.col - 1] = sensor.terreno[3];
					matriz[st.fil + 2][st.col] = sensor.terreno[4];
					matriz[st.fil + 2][st.col - 1] = sensor.terreno[5];
					matriz[st.fil + 2][st.col - 2] = sensor.terreno[6];
					matriz[st.fil + 1][st.col - 2] = sensor.terreno[7];
					matriz[st.fil][st.col - 2] = sensor.terreno[8];
					matriz[st.fil + 3][st.col] = sensor.terreno[9];
					matriz[st.fil + 3][st.col - 1] = sensor.terreno[10];
					matriz[st.fil + 3][st.col - 2] = sensor.terreno[11];
					matriz[st.fil + 3][st.col - 3] = sensor.terreno[12];
					matriz[st.fil + 2][st.col - 3] = sensor.terreno[13];
					matriz[st.fil + 1][st.col - 3] = sensor.terreno[14];
					matriz[st.fil][st.col - 3] = sensor.terreno[15];
					break;

				case oeste:
					matriz[st.fil + 1][st.col - 1] = sensor.terreno[1];
					matriz[st.fil][st.col - 1] = sensor.terreno[2];
					matriz[st.fil - 1][st.col - 1] = sensor.terreno[3];
					matriz[st.fil + 2][st.col - 2] = sensor.terreno[4];
					matriz[st.fil + 1][st.col - 2] = sensor.terreno[5];
					matriz[st.fil][st.col - 2] = sensor.terreno[6];
					matriz[st.fil - 1][st.col - 2] = sensor.terreno[7];
					matriz[st.fil - 2][st.col - 2] = sensor.terreno[8];
					matriz[st.fil + 3][st.col - 3] = sensor.terreno[9];
					matriz[st.fil + 2][st.col - 3] = sensor.terreno[10];
					matriz[st.fil + 1][st.col - 3] = sensor.terreno[11];
					matriz[st.fil][st.col - 3] = sensor.terreno[12];
					matriz[st.fil - 1][st.col - 3] = sensor.terreno[13];
					matriz[st.fil - 2][st.col - 3] = sensor.terreno[14];
					matriz[st.fil - 3][st.col - 3] = sensor.terreno[15];
					break;

				case noroeste:
					matriz[st.fil][st.col - 1] = sensor.terreno[1];
					matriz[st.fil - 1][st.col - 1] = sensor.terreno[2];
					matriz[st.fil - 1][st.col] = sensor.terreno[3];
					matriz[st.fil][st.col - 2] = sensor.terreno[4];
					matriz[st.fil - 1][st.col - 2] = sensor.terreno[5];
					matriz[st.fil - 2][st.col - 2] = sensor.terreno[6];
					matriz[st.fil - 2][st.col - 1] = sensor.terreno[7];
					matriz[st.fil - 2][st.col] = sensor.terreno[8];
					matriz[st.fil][st.col - 3] = sensor.terreno[9];
					matriz[st.fil - 1][st.col - 3] = sensor.terreno[10];
					matriz[st.fil - 2][st.col - 3] = sensor.terreno[11];
					matriz[st.fil - 3][st.col - 3] = sensor.terreno[12];
					matriz[st.fil - 3][st.col - 2] = sensor.terreno[13];
					matriz[st.fil - 3][st.col - 1] = sensor.terreno[14];
					matriz[st.fil - 3][st.col] = sensor.terreno[15];
					break;
		}
	}else {
		switch (st.brujula){
				case norte:
					matriz[st.fil - 1][st.col - 1] = sensor.terreno[1]; 
					matriz[st.fil - 1][st.col] = sensor.terreno[2];
					matriz[st.fil - 1][st.col + 1] = sensor.terreno[3];
					matriz[st.fil - 2][st.col - 2] = sensor.terreno[4];
					matriz[st.fil - 2][st.col - 1] = sensor.terreno[5];
					matriz[st.fil - 2][st.col + 1] = sensor.terreno[7];
					matriz[st.fil - 2][st.col + 2] = sensor.terreno[8];
					matriz[st.fil - 3][st.col - 3] = sensor.terreno[9];
					matriz[st.fil - 3][st.col - 2] = sensor.terreno[10];
					matriz[st.fil - 3][st.col + 2] = sensor.terreno[14];
					matriz[st.fil - 3][st.col + 3] = sensor.terreno[15];
					break;

				case noreste:
					matriz[st.fil - 1][st.col] = sensor.terreno[1];
					matriz[st.fil - 1][st.col + 1] = sensor.terreno[2];
					matriz[st.fil][st.col + 1] = sensor.terreno[3];
					matriz[st.fil - 2][st.col] = sensor.terreno[4];
					matriz[st.fil - 1][st.col + 1] = sensor.terreno[5];
					matriz[st.fil - 1][st.col + 2] = sensor.terreno[7];
					matriz[st.fil][st.col + 2] = sensor.terreno[8];
					matriz[st.fil - 3][st.col] = sensor.terreno[9];
					matriz[st.fil - 3][st.col + 1] = sensor.terreno[10];
					matriz[st.fil - 2][st.col + 3] = sensor.terreno[14];
					matriz[st.fil][st.col + 3] = sensor.terreno[15];
					break;

				case este:
					matriz[st.fil - 1][st.col + 1] = sensor.terreno[1];
					matriz[st.fil][st.col + 1] = sensor.terreno[2];
					matriz[st.fil + 1][st.col + 1] = sensor.terreno[3];
					matriz[st.fil - 2][st.col + 2] = sensor.terreno[4];
					matriz[st.fil - 1][st.col + 2] = sensor.terreno[5];
					matriz[st.fil + 1][st.col + 2] = sensor.terreno[7];
					matriz[st.fil + 2][st.col + 2] = sensor.terreno[8];
					matriz[st.fil - 3][st.col + 3] = sensor.terreno[9];
					matriz[st.fil - 2][st.col + 3] = sensor.terreno[10];
					matriz[st.fil + 2][st.col + 3] = sensor.terreno[14];
					matriz[st.fil + 3][st.col + 3] = sensor.terreno[15];
					break;

				case sureste:
					matriz[st.fil][st.col + 1] = sensor.terreno[1];
					matriz[st.fil + 1][st.col + 1] = sensor.terreno[2];
					matriz[st.fil+ 1][st.col] = sensor.terreno[3];
					matriz[st.fil][st.col + 2] = sensor.terreno[4];
					matriz[st.fil + 1][st.col + 2] = sensor.terreno[5];
					matriz[st.fil + 2][st.col + 1] = sensor.terreno[7];
					matriz[st.fil + 2][st.col] = sensor.terreno[8];
					matriz[st.fil][st.col + 3] = sensor.terreno[9];
					matriz[st.fil + 1][st.col + 3] = sensor.terreno[10];
					matriz[st.fil + 3][st.col + 1] = sensor.terreno[14];
					matriz[st.fil + 3][st.col] = sensor.terreno[15];
					break;

				case sur: 
					matriz[st.fil + 1][st.col + 1] = sensor.terreno[1];
					matriz[st.fil + 1][st.col] = sensor.terreno[2];
					matriz[st.fil + 1][st.col - 1] = sensor.terreno[3];
					matriz[st.fil + 2][st.col + 2] = sensor.terreno[4];
					matriz[st.fil + 2][st.col + 1] = sensor.terreno[5];
					matriz[st.fil + 2][st.col - 1] = sensor.terreno[7];
					matriz[st.fil + 2][st.col - 2] = sensor.terreno[8];
					matriz[st.fil + 3][st.col + 3] = sensor.terreno[9];
					matriz[st.fil + 3][st.col + 2] = sensor.terreno[10];
					matriz[st.fil + 3][st.col - 2] = sensor.terreno[14];
					matriz[st.fil + 3][st.col - 3] = sensor.terreno[15];
					break;

				case suroeste:
					matriz[st.fil + 1][st.col] = sensor.terreno[1];
					matriz[st.fil + 1][st.col - 1] = sensor.terreno[2];
					matriz[st.fil][st.col - 1] = sensor.terreno[3];
					matriz[st.fil + 2][st.col] = sensor.terreno[4];
					matriz[st.fil + 2][st.col - 1] = sensor.terreno[5];
					matriz[st.fil + 1][st.col - 2] = sensor.terreno[7];
					matriz[st.fil][st.col - 2] = sensor.terreno[8];
					matriz[st.fil + 3][st.col] = sensor.terreno[9];
					matriz[st.fil + 3][st.col - 1] = sensor.terreno[10];
					matriz[st.fil + 1][st.col - 3] = sensor.terreno[14];
					matriz[st.fil][st.col - 3] = sensor.terreno[15];
					break;

				case oeste:
					matriz[st.fil + 1][st.col - 1] = sensor.terreno[1];
					matriz[st.fil][st.col - 1] = sensor.terreno[2];
					matriz[st.fil - 1][st.col - 1] = sensor.terreno[3];
					matriz[st.fil + 2][st.col - 2] = sensor.terreno[4];
					matriz[st.fil + 1][st.col - 2] = sensor.terreno[5];
					matriz[st.fil - 1][st.col - 2] = sensor.terreno[7];
					matriz[st.fil - 2][st.col - 2] = sensor.terreno[8];
					matriz[st.fil + 3][st.col - 3] = sensor.terreno[9];
					matriz[st.fil + 2][st.col - 3] = sensor.terreno[10];
					matriz[st.fil - 2][st.col - 3] = sensor.terreno[14];
					matriz[st.fil - 3][st.col - 3] = sensor.terreno[15];
					break;

				case noroeste:
					matriz[st.fil][st.col - 1] = sensor.terreno[1];
					matriz[st.fil - 1][st.col - 1] = sensor.terreno[2];
					matriz[st.fil - 1][st.col] = sensor.terreno[3];
					matriz[st.fil][st.col - 2] = sensor.terreno[4];
					matriz[st.fil - 1][st.col - 2] = sensor.terreno[5];
					matriz[st.fil - 2][st.col - 1] = sensor.terreno[7];
					matriz[st.fil - 2][st.col] = sensor.terreno[8];
					matriz[st.fil][st.col - 3] = sensor.terreno[9];
					matriz[st.fil - 1][st.col - 3] = sensor.terreno[10];
					matriz[st.fil - 3][st.col - 1] = sensor.terreno[14];
					matriz[st.fil - 3][st.col] = sensor.terreno[15];
					break;
	}
}
}


int ComportamientoJugador::interact(Action accion, int valor){
	return false;
}
