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
				case noreste: current_state.fil--; current_state.col++; break;
				case noroeste: current_state.fil--; current_state.col--; break;
				case sureste: current_state.fil++; current_state.col++; break;
				case suroeste: current_state.fil++; current_state.col--; break;
			}
			break;
		case actTURN_SR:
			a = current_state.brujula;
			a = (a + 1) % 8;
			current_state.brujula = static_cast<Orientacion>(a);
			break;
		case actTURN_L:
			a = current_state.brujula;
			a = (a + 7) % 8;
			current_state.brujula = static_cast<Orientacion>(a);
			break;
	}

if (sensores.terreno[2] == 'T' or sensores.terreno[2] == 'S' and sensores.agentes[2] == '_'){
	accion = actWALK;
	}else 
		accion = actTURN_L;

last_action = accion;	
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}