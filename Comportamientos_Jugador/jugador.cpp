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

	
	//Pintamos Acantilados
	if (primera_vez){
		for (int i = 0; i < 3; ++i){
			for ( int j = 0; j < mapaResultado.size(); ++j)
				mapaResultado[i][j] = 'P';
		}

		for(int j = 0; j < 3; ++j){
			for (int i = 3; i < mapaResultado.size(); ++i){
				mapaResultado[i][j] = 'P';
			}
		}

		for (int i = mapaResultado.size() - 3; i < mapaResultado.size(); ++i){
			for (int j = 0; j < mapaResultado.size(); ++j){
				mapaResultado[i][j] = 'P';
			}
		}

		for (int j = mapaResultado.size() - 3; j < mapaResultado.size(); ++j){
			for (int i = 0; i < mapaResultado.size(); ++i){
				mapaResultado[i][j] = 'P';
			}
		}
		primera_vez = false;
	}
	
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

if (sensores.nivel == 0){
		current_state.fil = sensores.posF;
		current_state.col = sensores.posC;
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
					matriz[st.fil-1][st.col-1] = terreno[1];
					matriz[st.fil-1][st.col] = terreno[2];
					matriz[st.fil-1][st.col+1] = terreno[3];
					matriz[st.fil-2][st.col-2] = terreno[4];
					matriz[st.fil-2][st.col-1] = terreno[5];
					matriz[st.fil-2][st.col] = terreno[6];
					matriz[st.fil-2][st.col+1] = terreno[7];
					matriz[st.fil-2][st.col+2] = terreno[8];
					matriz[st.fil-3][st.col-3] = terreno[9];
					matriz[st.fil-3][st.col-2] = terreno[10];
					matriz[st.fil-3][st.col-1] = terreno[11];
					matriz[st.fil-3][st.col] = terreno[12];
					matriz[st.fil-3][st.col+1] = terreno[13];
					matriz[st.fil-3][st.col+2] = terreno[14];
					matriz[st.fil-3][st.col+3] = terreno[15];
				break;

				case noreste:
					matriz[st.fil-1][st.col] = terreno[1];
					matriz[st.fil-1][st.col+1] = terreno[2];
					matriz[st.fil][st.col+1] = terreno[3];
					matriz[st.fil-2][st.col] = terreno[4];
					matriz[st.fil-2][st.col+1] = terreno[5];
					matriz[st.fil-2][st.col+2] = terreno[6];
					matriz[st.fil-1][st.col+2] = terreno[7];
					matriz[st.fil][st.col+2] = terreno[8];
					matriz[st.fil-3][st.col] = terreno[9];
					matriz[st.fil-3][st.col+1] = terreno[10];
					matriz[st.fil-3][st.col+2] = terreno[11];
					matriz[st.fil-3][st.col+3] = terreno[12];
					matriz[st.fil-2][st.col+3] = terreno[13];
					matriz[st.fil-1][st.col+3] = terreno[14];
					matriz[st.fil][st.col+3] = terreno[15];
				break;

				case este:
					matriz[st.fil-1][st.col+1] = terreno[1];
					matriz[st.fil][st.col+1] = terreno[2];
					matriz[st.fil+1][st.col+1] = terreno[3];
					matriz[st.fil-2][st.col+2] = terreno[4];
					matriz[st.fil-1][st.col+2] = terreno[5];
					matriz[st.fil][st.col+2] = terreno[6];
					matriz[st.fil+1][st.col+2] = terreno[7];
					matriz[st.fil+2][st.col+2] = terreno[8];
					matriz[st.fil-3][st.col+3] = terreno[9];
					matriz[st.fil-2][st.col+3] = terreno[10];
					matriz[st.fil-1][st.col+3] = terreno[11];
					matriz[st.fil][st.col+3] = terreno[12];
					matriz[st.fil+1][st.col+3] = terreno[13];
					matriz[st.fil+2][st.col+3] = terreno[14];
					matriz[st.fil+3][st.col+3] = terreno[15];
				break;
				
				case sureste:
					matriz[st.fil][st.col+1] = terreno[1];
					matriz[st.fil+1][st.col+1] = terreno[2];
					matriz[st.fil+1][st.col] = terreno[3];
					matriz[st.fil][st.col+2] = terreno[4];
					matriz[st.fil+1][st.col+2] = terreno[5];
					matriz[st.fil+2][st.col+2] = terreno[6];
					matriz[st.fil+2][st.col+1] = terreno[7];
					matriz[st.fil+2][st.col] = terreno[8];
					matriz[st.fil][st.col+3] = terreno[9];
					matriz[st.fil+1][st.col+3] = terreno[10];
					matriz[st.fil+2][st.col+3] = terreno[11];
					matriz[st.fil+3][st.col+3] = terreno[12];
					matriz[st.fil+3][st.col+2] = terreno[13];
					matriz[st.fil+3][st.col+1] = terreno[14];
					matriz[st.fil+3][st.col] = terreno[15];
				break;

				case sur:
					matriz[st.fil+1][st.col+1] = terreno[1];
					matriz[st.fil+1][st.col] = terreno[2];
					matriz[st.fil+1][st.col-1] = terreno[3];
					matriz[st.fil+2][st.col+2] = terreno[4];
					matriz[st.fil+2][st.col+1] = terreno[5];
					matriz[st.fil+2][st.col] = terreno[6];
					matriz[st.fil+2][st.col-1] = terreno[7];
					matriz[st.fil+2][st.col-2] = terreno[8];
					matriz[st.fil+3][st.col+3] = terreno[9];
					matriz[st.fil+3][st.col+2] = terreno[10];
					matriz[st.fil+3][st.col+1] = terreno[11];
					matriz[st.fil+3][st.col] = terreno[12];
					matriz[st.fil+3][st.col-1] = terreno[13];
					matriz[st.fil+3][st.col-2] = terreno[14];
					matriz[st.fil+3][st.col-3] = terreno[15];
				break;

				case suroeste:
					matriz[st.fil+1][st.col] = terreno[1];
					matriz[st.fil+1][st.col-1] = terreno[2];
					matriz[st.fil][st.col-1] = terreno[3];
					matriz[st.fil+2][st.col] = terreno[4];
					matriz[st.fil+2][st.col-1] = terreno[5];
					matriz[st.fil+2][st.col-2] = terreno[6];
					matriz[st.fil+1][st.col-2] = terreno[7];
					matriz[st.fil][st.col-2] = terreno[8];
					matriz[st.fil+3][st.col] = terreno[9];
					matriz[st.fil+3][st.col-1] = terreno[10];
					matriz[st.fil+3][st.col-2] = terreno[11];
					matriz[st.fil+3][st.col-3] = terreno[12];
					matriz[st.fil+2][st.col-3] = terreno[13];
					matriz[st.fil+1][st.col-3] = terreno[14];
					matriz[st.fil][st.col-3] = terreno[15];
				break;

				case oeste:
					matriz[st.fil+1][st.col-1] = terreno[1];
					matriz[st.fil][st.col-1] = terreno[2];
					matriz[st.fil-1][st.col-1] = terreno[3];
					matriz[st.fil+2][st.col-2] = terreno[4];
					matriz[st.fil+1][st.col-2] = terreno[5];
					matriz[st.fil][st.col-2] = terreno[6];
					matriz[st.fil-1][st.col-2] = terreno[7];
					matriz[st.fil-2][st.col-2] = terreno[8];
					matriz[st.fil+3][st.col-3] = terreno[9];
					matriz[st.fil+2][st.col-3] = terreno[10];
					matriz[st.fil+1][st.col-3] = terreno[11];
					matriz[st.fil][st.col-3] = terreno[12];
					matriz[st.fil-1][st.col-3] = terreno[13];
					matriz[st.fil-2][st.col-3] = terreno[14];
					matriz[st.fil-3][st.col-3] = terreno[15];
				break;

				case noroeste:
					matriz[st.fil][st.col-1] = terreno[1];
					matriz[st.fil-1][st.col-1] = terreno[2];
					matriz[st.fil-1][st.col] = terreno[3];
					matriz[st.fil][st.col-2] = terreno[4];
					matriz[st.fil-1][st.col-2] = terreno[5];
					matriz[st.fil-2][st.col-2] = terreno[6];
					matriz[st.fil-2][st.col-1] = terreno[7];
					matriz[st.fil-2][st.col] = terreno[8];
					matriz[st.fil][st.col-3] = terreno[9];
					matriz[st.fil-1][st.col-3] = terreno[10];
					matriz[st.fil-2][st.col-3] = terreno[11];
					matriz[st.fil-3][st.col-3] = terreno[12];
					matriz[st.fil-3][st.col-2] = terreno[13];
					matriz[st.fil-3][st.col-1] = terreno[14];
					matriz[st.fil-3][st.col] = terreno[15];
				break;
		}
	}else {
		switch (st.brujula){
				case norte:
					matriz[st.fil-1][st.col-1] = terreno[1];
					matriz[st.fil-1][st.col] = terreno[2];
					matriz[st.fil-1][st.col+1] = terreno[3];
					matriz[st.fil-2][st.col-2] = terreno[4];
					matriz[st.fil-2][st.col-1] = terreno[5];
					matriz[st.fil-2][st.col+1] = terreno[7];
					matriz[st.fil-2][st.col+2] = terreno[8];
					matriz[st.fil-3][st.col-3] = terreno[9];
					matriz[st.fil-3][st.col-2] = terreno[10];
					matriz[st.fil-3][st.col+2] = terreno[14];
					matriz[st.fil-3][st.col+3] = terreno[15];
				break;

				case noreste:
					matriz[st.fil-1][st.col] = terreno[1];
					matriz[st.fil-1][st.col+1] = terreno[2];
					matriz[st.fil][st.col+1] = terreno[3];
					matriz[st.fil-2][st.col] = terreno[4];
					matriz[st.fil-2][st.col+1] = terreno[5];
					matriz[st.fil-1][st.col+2] = terreno[7];
					matriz[st.fil][st.col+2] = terreno[8];
					matriz[st.fil-3][st.col] = terreno[9];
					matriz[st.fil-3][st.col+1] = terreno[10];
					matriz[st.fil-1][st.col+3] = terreno[14];
					matriz[st.fil][st.col+3] = terreno[15];
				break;

				case este:
					matriz[st.fil-1][st.col+1] = terreno[1];
					matriz[st.fil][st.col+1] = terreno[2];
					matriz[st.fil+1][st.col+1] = terreno[3];
					matriz[st.fil-2][st.col+2] = terreno[4];
					matriz[st.fil-1][st.col+2] = terreno[5];
					matriz[st.fil+1][st.col+2] = terreno[7];
					matriz[st.fil+2][st.col+2] = terreno[8];
					matriz[st.fil-3][st.col+3] = terreno[9];
					matriz[st.fil-2][st.col+3] = terreno[10];
					matriz[st.fil+2][st.col+3] = terreno[14];
					matriz[st.fil+3][st.col+3] = terreno[15];
				break;
				
				case sureste:
					matriz[st.fil][st.col+1] = terreno[1];
					matriz[st.fil+1][st.col+1] = terreno[2];
					matriz[st.fil+1][st.col] = terreno[3];
					matriz[st.fil][st.col+2] = terreno[4];
					matriz[st.fil+1][st.col+2] = terreno[5];
					matriz[st.fil+2][st.col+1] = terreno[7];
					matriz[st.fil+2][st.col] = terreno[8];
					matriz[st.fil][st.col+3] = terreno[9];
					matriz[st.fil+1][st.col+3] = terreno[10];
					matriz[st.fil+3][st.col+1] = terreno[14];
					matriz[st.fil+3][st.col] = terreno[15];
				break;

				case sur:
					matriz[st.fil+1][st.col+1] = terreno[1];
					matriz[st.fil+1][st.col] = terreno[2];
					matriz[st.fil+1][st.col-1] = terreno[3];
					matriz[st.fil+2][st.col+2] = terreno[4];
					matriz[st.fil+2][st.col+1] = terreno[5];
					matriz[st.fil+2][st.col-1] = terreno[7];
					matriz[st.fil+2][st.col-2] = terreno[8];
					matriz[st.fil+3][st.col+3] = terreno[9];
					matriz[st.fil+3][st.col+2] = terreno[10];
					matriz[st.fil+3][st.col-2] = terreno[14];
					matriz[st.fil+3][st.col-3] = terreno[15];
				break;

				case suroeste:
					matriz[st.fil+1][st.col] = terreno[1];
					matriz[st.fil+1][st.col-1] = terreno[2];
					matriz[st.fil][st.col-1] = terreno[3];
					matriz[st.fil+2][st.col] = terreno[4];
					matriz[st.fil+2][st.col-1] = terreno[5];
					matriz[st.fil+1][st.col-2] = terreno[7];
					matriz[st.fil][st.col-2] = terreno[8];
					matriz[st.fil+3][st.col] = terreno[9];
					matriz[st.fil+3][st.col-1] = terreno[10];
					matriz[st.fil+1][st.col-3] = terreno[14];
					matriz[st.fil][st.col-3] = terreno[15];
				break;

				case oeste:
					matriz[st.fil+1][st.col-1] = terreno[1];
					matriz[st.fil][st.col-1] = terreno[2];
					matriz[st.fil-1][st.col-1] = terreno[3];
					matriz[st.fil+2][st.col-2] = terreno[4];
					matriz[st.fil+1][st.col-2] = terreno[5];
					matriz[st.fil-1][st.col-2] = terreno[7];
					matriz[st.fil-2][st.col-2] = terreno[8];
					matriz[st.fil+3][st.col-3] = terreno[9];
					matriz[st.fil+2][st.col-3] = terreno[10];
					matriz[st.fil-2][st.col-3] = terreno[14];
					matriz[st.fil-3][st.col-3] = terreno[15];
				break;

				case noroeste:
					matriz[st.fil][st.col-1] = terreno[1];
					matriz[st.fil-1][st.col-1] = terreno[2];
					matriz[st.fil-1][st.col] = terreno[3];
					matriz[st.fil][st.col-2] = terreno[4];
					matriz[st.fil-1][st.col-2] = terreno[5];
					matriz[st.fil-2][st.col-1] = terreno[7];
					matriz[st.fil-2][st.col] = terreno[8];
					matriz[st.fil][st.col-3] = terreno[9];
					matriz[st.fil-1][st.col-3] = terreno[10];
					matriz[st.fil-3][st.col-1] = terreno[14];
					matriz[st.fil-3][st.col] = terreno[15];
				break;
		}
	}
}


int ComportamientoJugador::interact(Action accion, int valor){
	return false;
}

int ComportamientoJugador::HayCasilaEspecialNecesariaEnVista(const vector<unsigned char> &terreno,state posicion){
	int pos = -1, pos_posicion = -1, pos_recuperacion = -1, pos_bikini = -1, pos_zapatillas = -1;
	hay_algo = false;
	hay_casilla_posicion = false;
	hay_casilla_recuperacion = false;
	hay_casilla_bikini = false;
	hay_casilla_zapatillas = false;

	for (int i = 0; i < 15; ++i){
		if (terreno[i] == 'K' and !bien_situado){
			hay_casilla_posicion = true;
			hay_algo = true;
			pos_posicion = i;
		}
		if (terreno[i] == 'X'){
			hay_casilla_recuperacion = true;
			hay_algo = true;
			pos_recuperacion = i;
		}
		if (terreno[i] == 'K' and !tengo_bikini){
			hay_casilla_bikini = true;
			hay_algo = true;
			pos_bikini = i;
			if (i == 0){
				tengo_bikini = true;
			}
		}
		if (terreno[i] == 'D' and !tengo_zapatilllas){
			hay_casilla_zapatillas = true;
			hay_algo = true;
			pos_zapatillas = i;
			if (i == 0){
				tengo_zapatilllas = true;
			}
		}
	}

	if (hay_casilla_posicion and !bien_situado){
		pos = pos_posicion;
	}else if (hay_casilla_recuperacion and busco_bateria){
		pos = pos_recuperacion;
	}else if (hay_casilla_zapatillas and !tengo_zapatilllas){
		pos = pos_zapatillas;
	}else if (hay_casilla_bikini and !tengo_bikini){
		pos = pos_bikini;
	}
	return pos;
}
