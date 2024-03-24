#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;

Action ComportamientoJugador::think(Sensores sensores)
{

	Action accion = actIDLE;

	// Mostrar el valor de los sensores
	cout << "Posicion: fila " << sensores.posF << " columna " << sensores.posC;
	switch (sensores.sentido)
	{
	case norte:
		cout << " Norte\n";
		break;
	case noreste:
		cout << " Noreste\n";
		break;
	case este:
		cout << " Este\n";
		break;
	case sureste:
		cout << " Sureste\n";
		break;
	case sur:
		cout << " Sur\n";
		break;
	case suroeste:
		cout << " Suroeste\n";
		break;
	case oeste:
		cout << " Oeste\n";
		break;
	case noroeste:
		cout << " Noroeste\n";
		break;
	}
	cout << "Terreno: ";
	for (int i = 0; i < sensores.terreno.size(); i++)
		cout << sensores.terreno[i];

	cout << "  Agentes: ";
	for (int i = 0; i < sensores.agentes.size(); i++)
		cout << sensores.agentes[i];

	cout << "\nColision: " << sensores.colision;
	cout << "  Reset: " << sensores.reset;
	cout << "  Vida: " << sensores.vida << endl
		 << endl;

	// Pintamos Acantilados
	PintarPrecicpicios(primera_vez);

	switch (last_action)
	{

	case actWALK:
		if (sensores.colision == false)
		{
			switch (current_state.brujula)
			{
			case norte:
				current_state.fil--;
				break;
			case sur:
				current_state.fil++;
				break;
			case este:
				current_state.col++;
				break;
			case oeste:
				current_state.col--;
				break;
			case noreste:
				current_state.fil--;
				current_state.col++;
				break;
			case noroeste:
				current_state.fil--;
				current_state.col--;
				break;
			case sureste:
				current_state.fil++;
				current_state.col++;
				break;
			case suroeste:
				current_state.fil++;
				current_state.col--;
				break;
			}
		}
		break;

	case actRUN:
		if (sensores.colision == false)
		{
			switch (current_state.brujula)
			{
			case norte:
				current_state.fil -= 2;
				break;
			case sur:
				current_state.fil += 2;
				break;
			case este:
				current_state.col += 2;
				break;
			case oeste:
				current_state.col -= 2;
				break;
			case noreste:
				current_state.fil -= 2;
				current_state.col += 2;
				break;
			case noroeste:
				current_state.fil -= 2;
				current_state.col -= 2;
				break;
			case sureste:
				current_state.fil += 2;
				current_state.col += 2;
				break;
			case suroeste:
				current_state.fil += 2;
				current_state.col -= 2;
				break;
			}
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

	if (sensores.reset == true)
	{
		tengo_bikini = false;
		tengo_zapatilllas = false;
		if (sensores.nivel != 0)
		{
			bien_situado = false;
		}
	}

	if (sensores.nivel == 0)
	{
		current_state.fil = sensores.posF;
		current_state.col = sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado = true;
	}
	else if (sensores.nivel == 1 or sensores.nivel == 2)
		current_state.brujula = norte;
	if (sensores.terreno[0] == 'G' and !bien_situado)
	{
		current_state.fil = sensores.posF;
		current_state.col = sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado = true;
		cout << "Bien posicionado" << endl;
		fil = 99;
		col = 99;
	}
	if (!bien_situado)
	{
		PonerTerrenoMatrizNoSituado(sensores.terreno, current_state, mapaNoSituado, current_state.brujula, sensores);
	}

	if (bien_situado)
	{
		PonerTerrenoMatriz(sensores.terreno, current_state, mapaResultado, current_state.brujula, sensores);
		PonerCantidadEnAuxiliar(current_state, mapaPasos, current_state.brujula, sensores);
	}
	pos = HayCasilaEspecialNecesariaEnVista(sensores.terreno, sensores);

	if (hay_algo and pos != 0)
	{
		accion = IrCasillaEnVista(pos, sensores.terreno);
	}
	else
	{
		if (sensores.terreno[2] == 'T' or sensores.terreno[2] == 'S' or sensores.terreno[2] == 'G' and sensores.agentes[2] == '_')
		{
			accion = actWALK;
		}
		else if (!girar_derecha)
		{
			accion = actTURN_L;
			girar_derecha = (rand() % 2 == 0);
		}
		else
		{
			accion = actTURN_SR;
			girar_derecha = (rand() % 2 == 0);
		}
	}

	last_action = accion;
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}

int ComportamientoJugador::HayCasilaEspecialNecesariaEnVista(const vector<unsigned char> &terreno, Sensores sensor)
{
	int pos = -1, pos_posicion = -1, pos_recuperacion = -1, pos_bikini = -1, pos_zapatillas = -1;
	hay_algo = false;
	hay_casilla_posicion = false;
	hay_casilla_recuperacion = false;
	hay_casilla_bikini = false;
	hay_casilla_zapatillas = false;

	for (int i = 0; i <= 15; ++i)
	{
		if (terreno[i] == 'G' and !bien_situado)
		{
			hay_casilla_posicion = true;
			hay_algo = true;
			if (pos_posicion == -1)
				pos_posicion = i;
			else if (pos_posicion > i)
				pos_posicion = i;
		}
		if (terreno[i] == 'X')
		{
			hay_casilla_recuperacion = true;
			hay_algo = true;
			if (pos_recuperacion == -1)
				pos_recuperacion = i;
			else if (pos_recuperacion > i)
				pos_recuperacion = i;
		}
		if (terreno[i] == 'K' and !tengo_bikini)
		{
			hay_casilla_bikini = true;
			hay_algo = true;
			if (pos_bikini == -1)
				pos_bikini = i;
			else if (pos_bikini > i)
				pos_bikini = i;

			if (i == 0)
			{
				tengo_bikini = true;
			}
		}
		if (terreno[i] == 'D' and !tengo_zapatilllas)
		{
			hay_casilla_zapatillas = true;
			hay_algo = true;
			if (pos_zapatillas == -1)
				pos_zapatillas = i;
			else if (pos_zapatillas > i)
				pos_recuperacion = i;

			if (i == 0)
			{
				tengo_zapatilllas = true;
			}
		}
	}

	if (hay_casilla_posicion and !bien_situado)
	{
		pos = pos_posicion;
	}
	else if (hay_casilla_recuperacion and busco_bateria)
	{
		pos = pos_recuperacion;
	}
	else if (hay_casilla_zapatillas and !tengo_zapatilllas)
	{
		pos = pos_zapatillas;
	}
	else if (hay_casilla_bikini and !tengo_bikini)
	{
		pos = pos_bikini;
	}

	if (sensor.nivel == 3 and (pos == 6 or pos == 11 or pos == 12 or pos == 13))
	{
		pos = -1;
		hay_algo = false;
	}

	return pos;
}

Action ComportamientoJugador::IrCasillaEnVista(int pos, const vector<unsigned char> &terreno)
{
	imposible = false;
	Action accion;
	if (pos == 1 or pos == 4 or pos == 9)
		accion = actTURN_L;
	else if (pos == 3 or pos == 8 or pos == 15)
		accion = actTURN_SR;
	else if (terreno[2] != 'M' or terreno[2] != 'P')
		accion = actWALK;
	else
		imposible = true;
	return accion;
}

void ComportamientoJugador::PintarPrecicpicios(bool pintar)
{
	if (pintar)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < mapaResultado.size(); ++j)
				mapaResultado[i][j] = 'P';
		}

		for (int j = 0; j < 3; ++j)
		{
			for (int i = 3; i < mapaResultado.size(); ++i)
			{
				mapaResultado[i][j] = 'P';
			}
		}

		for (int i = mapaResultado.size() - 3; i < mapaResultado.size(); ++i)
		{
			for (int j = 0; j < mapaResultado.size(); ++j)
			{
				mapaResultado[i][j] = 'P';
			}
		}

		for (int j = mapaResultado.size() - 3; j < mapaResultado.size(); ++j)
		{
			for (int i = 0; i < mapaResultado.size(); ++i)
			{
				mapaResultado[i][j] = 'P';
			}
		}
		pintar = false;
	}
}

void ComportamientoJugador::PonerTerrenoMatriz(const vector<unsigned char> &terreno, const state &st, vector<vector<unsigned char>> &matriz, Orientacion pos, Sensores sensor)
{
	if (st.fil < 0 || st.fil >= matriz.size() || st.col < 0 || st.col >= matriz[0].size())
	{
		cout << "Error: Índices fuera de los límites de la matriz." << endl;
		return;
	}
	matriz[st.fil][st.col] = terreno[0];
	if (sensor.nivel != 3)
	{
		switch (st.brujula)
		{
		case norte:
			matriz[st.fil - 1][st.col - 1] = terreno[1];
			matriz[st.fil - 1][st.col] = terreno[2];
			matriz[st.fil - 1][st.col + 1] = terreno[3];
			matriz[st.fil - 2][st.col - 2] = terreno[4];
			matriz[st.fil - 2][st.col - 1] = terreno[5];
			matriz[st.fil - 2][st.col] = terreno[6];
			matriz[st.fil - 2][st.col + 1] = terreno[7];
			matriz[st.fil - 2][st.col + 2] = terreno[8];
			matriz[st.fil - 3][st.col - 3] = terreno[9];
			matriz[st.fil - 3][st.col - 2] = terreno[10];
			matriz[st.fil - 3][st.col - 1] = terreno[11];
			matriz[st.fil - 3][st.col] = terreno[12];
			matriz[st.fil - 3][st.col + 1] = terreno[13];
			matriz[st.fil - 3][st.col + 2] = terreno[14];
			matriz[st.fil - 3][st.col + 3] = terreno[15];
			break;

		case noreste:
			matriz[st.fil - 1][st.col] = terreno[1];
			matriz[st.fil - 1][st.col + 1] = terreno[2];
			matriz[st.fil][st.col + 1] = terreno[3];
			matriz[st.fil - 2][st.col] = terreno[4];
			matriz[st.fil - 2][st.col + 1] = terreno[5];
			matriz[st.fil - 2][st.col + 2] = terreno[6];
			matriz[st.fil - 1][st.col + 2] = terreno[7];
			matriz[st.fil][st.col + 2] = terreno[8];
			matriz[st.fil - 3][st.col] = terreno[9];
			matriz[st.fil - 3][st.col + 1] = terreno[10];
			matriz[st.fil - 3][st.col + 2] = terreno[11];
			matriz[st.fil - 3][st.col + 3] = terreno[12];
			matriz[st.fil - 2][st.col + 3] = terreno[13];
			matriz[st.fil - 1][st.col + 3] = terreno[14];
			matriz[st.fil][st.col + 3] = terreno[15];
			break;

		case este:
			matriz[st.fil - 1][st.col + 1] = terreno[1];
			matriz[st.fil][st.col + 1] = terreno[2];
			matriz[st.fil + 1][st.col + 1] = terreno[3];
			matriz[st.fil - 2][st.col + 2] = terreno[4];
			matriz[st.fil - 1][st.col + 2] = terreno[5];
			matriz[st.fil][st.col + 2] = terreno[6];
			matriz[st.fil + 1][st.col + 2] = terreno[7];
			matriz[st.fil + 2][st.col + 2] = terreno[8];
			matriz[st.fil - 3][st.col + 3] = terreno[9];
			matriz[st.fil - 2][st.col + 3] = terreno[10];
			matriz[st.fil - 1][st.col + 3] = terreno[11];
			matriz[st.fil][st.col + 3] = terreno[12];
			matriz[st.fil + 1][st.col + 3] = terreno[13];
			matriz[st.fil + 2][st.col + 3] = terreno[14];
			matriz[st.fil + 3][st.col + 3] = terreno[15];
			break;

		case sureste:
			matriz[st.fil][st.col + 1] = terreno[1];
			matriz[st.fil + 1][st.col + 1] = terreno[2];
			matriz[st.fil + 1][st.col] = terreno[3];
			matriz[st.fil][st.col + 2] = terreno[4];
			matriz[st.fil + 1][st.col + 2] = terreno[5];
			matriz[st.fil + 2][st.col + 2] = terreno[6];
			matriz[st.fil + 2][st.col + 1] = terreno[7];
			matriz[st.fil + 2][st.col] = terreno[8];
			matriz[st.fil][st.col + 3] = terreno[9];
			matriz[st.fil + 1][st.col + 3] = terreno[10];
			matriz[st.fil + 2][st.col + 3] = terreno[11];
			matriz[st.fil + 3][st.col + 3] = terreno[12];
			matriz[st.fil + 3][st.col + 2] = terreno[13];
			matriz[st.fil + 3][st.col + 1] = terreno[14];
			matriz[st.fil + 3][st.col] = terreno[15];
			break;

		case sur:
			matriz[st.fil + 1][st.col + 1] = terreno[1];
			matriz[st.fil + 1][st.col] = terreno[2];
			matriz[st.fil + 1][st.col - 1] = terreno[3];
			matriz[st.fil + 2][st.col + 2] = terreno[4];
			matriz[st.fil + 2][st.col + 1] = terreno[5];
			matriz[st.fil + 2][st.col] = terreno[6];
			matriz[st.fil + 2][st.col - 1] = terreno[7];
			matriz[st.fil + 2][st.col - 2] = terreno[8];
			matriz[st.fil + 3][st.col + 3] = terreno[9];
			matriz[st.fil + 3][st.col + 2] = terreno[10];
			matriz[st.fil + 3][st.col + 1] = terreno[11];
			matriz[st.fil + 3][st.col] = terreno[12];
			matriz[st.fil + 3][st.col - 1] = terreno[13];
			matriz[st.fil + 3][st.col - 2] = terreno[14];
			matriz[st.fil + 3][st.col - 3] = terreno[15];
			break;

		case suroeste:
			matriz[st.fil + 1][st.col] = terreno[1];
			matriz[st.fil + 1][st.col - 1] = terreno[2];
			matriz[st.fil][st.col - 1] = terreno[3];
			matriz[st.fil + 2][st.col] = terreno[4];
			matriz[st.fil + 2][st.col - 1] = terreno[5];
			matriz[st.fil + 2][st.col - 2] = terreno[6];
			matriz[st.fil + 1][st.col - 2] = terreno[7];
			matriz[st.fil][st.col - 2] = terreno[8];
			matriz[st.fil + 3][st.col] = terreno[9];
			matriz[st.fil + 3][st.col - 1] = terreno[10];
			matriz[st.fil + 3][st.col - 2] = terreno[11];
			matriz[st.fil + 3][st.col - 3] = terreno[12];
			matriz[st.fil + 2][st.col - 3] = terreno[13];
			matriz[st.fil + 1][st.col - 3] = terreno[14];
			matriz[st.fil][st.col - 3] = terreno[15];
			break;

		case oeste:
			matriz[st.fil + 1][st.col - 1] = terreno[1];
			matriz[st.fil][st.col - 1] = terreno[2];
			matriz[st.fil - 1][st.col - 1] = terreno[3];
			matriz[st.fil + 2][st.col - 2] = terreno[4];
			matriz[st.fil + 1][st.col - 2] = terreno[5];
			matriz[st.fil][st.col - 2] = terreno[6];
			matriz[st.fil - 1][st.col - 2] = terreno[7];
			matriz[st.fil - 2][st.col - 2] = terreno[8];
			matriz[st.fil + 3][st.col - 3] = terreno[9];
			matriz[st.fil + 2][st.col - 3] = terreno[10];
			matriz[st.fil + 1][st.col - 3] = terreno[11];
			matriz[st.fil][st.col - 3] = terreno[12];
			matriz[st.fil - 1][st.col - 3] = terreno[13];
			matriz[st.fil - 2][st.col - 3] = terreno[14];
			matriz[st.fil - 3][st.col - 3] = terreno[15];
			break;

		case noroeste:
			matriz[st.fil][st.col - 1] = terreno[1];
			matriz[st.fil - 1][st.col - 1] = terreno[2];
			matriz[st.fil - 1][st.col] = terreno[3];
			matriz[st.fil][st.col - 2] = terreno[4];
			matriz[st.fil - 1][st.col - 2] = terreno[5];
			matriz[st.fil - 2][st.col - 2] = terreno[6];
			matriz[st.fil - 2][st.col - 1] = terreno[7];
			matriz[st.fil - 2][st.col] = terreno[8];
			matriz[st.fil][st.col - 3] = terreno[9];
			matriz[st.fil - 1][st.col - 3] = terreno[10];
			matriz[st.fil - 2][st.col - 3] = terreno[11];
			matriz[st.fil - 3][st.col - 3] = terreno[12];
			matriz[st.fil - 3][st.col - 2] = terreno[13];
			matriz[st.fil - 3][st.col - 1] = terreno[14];
			matriz[st.fil - 3][st.col] = terreno[15];
			break;
		}
	}
	else
	{
		switch (st.brujula)
		{
		case norte:
			matriz[st.fil - 1][st.col - 1] = terreno[1];
			matriz[st.fil - 1][st.col] = terreno[2];
			matriz[st.fil - 1][st.col + 1] = terreno[3];
			matriz[st.fil - 2][st.col - 2] = terreno[4];
			matriz[st.fil - 2][st.col - 1] = terreno[5];
			matriz[st.fil - 2][st.col + 1] = terreno[7];
			matriz[st.fil - 2][st.col + 2] = terreno[8];
			matriz[st.fil - 3][st.col - 3] = terreno[9];
			matriz[st.fil - 3][st.col - 2] = terreno[10];
			matriz[st.fil - 3][st.col + 2] = terreno[14];
			matriz[st.fil - 3][st.col + 3] = terreno[15];
			break;

		case noreste:
			matriz[st.fil - 1][st.col] = terreno[1];
			matriz[st.fil - 1][st.col + 1] = terreno[2];
			matriz[st.fil][st.col + 1] = terreno[3];
			matriz[st.fil - 2][st.col] = terreno[4];
			matriz[st.fil - 2][st.col + 1] = terreno[5];
			matriz[st.fil - 1][st.col + 2] = terreno[7];
			matriz[st.fil][st.col + 2] = terreno[8];
			matriz[st.fil - 3][st.col] = terreno[9];
			matriz[st.fil - 3][st.col + 1] = terreno[10];
			matriz[st.fil - 1][st.col + 3] = terreno[14];
			matriz[st.fil][st.col + 3] = terreno[15];
			break;

		case este:
			matriz[st.fil - 1][st.col + 1] = terreno[1];
			matriz[st.fil][st.col + 1] = terreno[2];
			matriz[st.fil + 1][st.col + 1] = terreno[3];
			matriz[st.fil - 2][st.col + 2] = terreno[4];
			matriz[st.fil - 1][st.col + 2] = terreno[5];
			matriz[st.fil + 1][st.col + 2] = terreno[7];
			matriz[st.fil + 2][st.col + 2] = terreno[8];
			matriz[st.fil - 3][st.col + 3] = terreno[9];
			matriz[st.fil - 2][st.col + 3] = terreno[10];
			matriz[st.fil + 2][st.col + 3] = terreno[14];
			matriz[st.fil + 3][st.col + 3] = terreno[15];
			break;

		case sureste:
			matriz[st.fil][st.col + 1] = terreno[1];
			matriz[st.fil + 1][st.col + 1] = terreno[2];
			matriz[st.fil + 1][st.col] = terreno[3];
			matriz[st.fil][st.col + 2] = terreno[4];
			matriz[st.fil + 1][st.col + 2] = terreno[5];
			matriz[st.fil + 2][st.col + 1] = terreno[7];
			matriz[st.fil + 2][st.col] = terreno[8];
			matriz[st.fil][st.col + 3] = terreno[9];
			matriz[st.fil + 1][st.col + 3] = terreno[10];
			matriz[st.fil + 3][st.col + 1] = terreno[14];
			matriz[st.fil + 3][st.col] = terreno[15];
			break;

		case sur:
			matriz[st.fil + 1][st.col + 1] = terreno[1];
			matriz[st.fil + 1][st.col] = terreno[2];
			matriz[st.fil + 1][st.col - 1] = terreno[3];
			matriz[st.fil + 2][st.col + 2] = terreno[4];
			matriz[st.fil + 2][st.col + 1] = terreno[5];
			matriz[st.fil + 2][st.col - 1] = terreno[7];
			matriz[st.fil + 2][st.col - 2] = terreno[8];
			matriz[st.fil + 3][st.col + 3] = terreno[9];
			matriz[st.fil + 3][st.col + 2] = terreno[10];
			matriz[st.fil + 3][st.col - 2] = terreno[14];
			matriz[st.fil + 3][st.col - 3] = terreno[15];
			break;

		case suroeste:
			matriz[st.fil + 1][st.col] = terreno[1];
			matriz[st.fil + 1][st.col - 1] = terreno[2];
			matriz[st.fil][st.col - 1] = terreno[3];
			matriz[st.fil + 2][st.col] = terreno[4];
			matriz[st.fil + 2][st.col - 1] = terreno[5];
			matriz[st.fil + 1][st.col - 2] = terreno[7];
			matriz[st.fil][st.col - 2] = terreno[8];
			matriz[st.fil + 3][st.col] = terreno[9];
			matriz[st.fil + 3][st.col - 1] = terreno[10];
			matriz[st.fil + 1][st.col - 3] = terreno[14];
			matriz[st.fil][st.col - 3] = terreno[15];
			break;

		case oeste:
			matriz[st.fil + 1][st.col - 1] = terreno[1];
			matriz[st.fil][st.col - 1] = terreno[2];
			matriz[st.fil - 1][st.col - 1] = terreno[3];
			matriz[st.fil + 2][st.col - 2] = terreno[4];
			matriz[st.fil + 1][st.col - 2] = terreno[5];
			matriz[st.fil - 1][st.col - 2] = terreno[7];
			matriz[st.fil - 2][st.col - 2] = terreno[8];
			matriz[st.fil + 3][st.col - 3] = terreno[9];
			matriz[st.fil + 2][st.col - 3] = terreno[10];
			matriz[st.fil - 2][st.col - 3] = terreno[14];
			matriz[st.fil - 3][st.col - 3] = terreno[15];
			break;

		case noroeste:
			matriz[st.fil][st.col - 1] = terreno[1];
			matriz[st.fil - 1][st.col - 1] = terreno[2];
			matriz[st.fil - 1][st.col] = terreno[3];
			matriz[st.fil][st.col - 2] = terreno[4];
			matriz[st.fil - 1][st.col - 2] = terreno[5];
			matriz[st.fil - 2][st.col - 1] = terreno[7];
			matriz[st.fil - 2][st.col] = terreno[8];
			matriz[st.fil][st.col - 3] = terreno[9];
			matriz[st.fil - 1][st.col - 3] = terreno[10];
			matriz[st.fil - 3][st.col - 1] = terreno[14];
			matriz[st.fil - 3][st.col] = terreno[15];
			break;
		}
	}
}

void ComportamientoJugador::PonerCantidadEnAuxiliar(const state &st, vector<vector<int>> &matriz, Orientacion pos, Sensores sensor)
{
	if (st.fil < 0 || st.fil >= matriz.size() || st.col < 0 || st.col >= matriz[0].size())
	{
		cout << "Error: Índices fuera de los límites de la matriz." << endl;
		return;
	}
	matriz[st.fil][st.col] += 1;
	if (sensor.nivel != 3)
	{
		switch (st.brujula)
		{
		case norte:
			matriz[st.fil - 1][st.col - 1] += 1;
			matriz[st.fil - 1][st.col] += 1;
			matriz[st.fil - 1][st.col + 1] += 1;
			matriz[st.fil - 2][st.col - 2] += 1;
			matriz[st.fil - 2][st.col - 1] += 1;
			matriz[st.fil - 2][st.col] += 1;
			matriz[st.fil - 2][st.col + 1] += 1;
			matriz[st.fil - 2][st.col + 2] += 1;
			matriz[st.fil - 3][st.col - 3] += 1;
			matriz[st.fil - 3][st.col - 2] += 1;
			matriz[st.fil - 3][st.col - 1] += 1;
			matriz[st.fil - 3][st.col] += 1;
			matriz[st.fil - 3][st.col + 1] += 1;
			matriz[st.fil - 3][st.col + 2] += 1;
			matriz[st.fil - 3][st.col + 3] += 1;
			break;

		case noreste:
			matriz[st.fil - 1][st.col] += 1;
			matriz[st.fil - 1][st.col + 1] += 1;
			matriz[st.fil][st.col + 1] += 1;
			matriz[st.fil - 2][st.col] += 1;
			matriz[st.fil - 2][st.col + 1] += 1;
			matriz[st.fil - 2][st.col + 2] += 1;
			matriz[st.fil - 1][st.col + 2] += 1;
			matriz[st.fil][st.col + 2] += 1;
			matriz[st.fil - 3][st.col] += 1;
			matriz[st.fil - 3][st.col + 1] += 1;
			matriz[st.fil - 3][st.col + 2] += 1;
			matriz[st.fil - 3][st.col + 3] += 1;
			matriz[st.fil - 2][st.col + 3] += 1;
			matriz[st.fil - 1][st.col + 3] += 1;
			matriz[st.fil][st.col + 3] += 1;
			break;

		case este:
			matriz[st.fil - 1][st.col + 1] += 1;
			matriz[st.fil][st.col + 1] += 1;
			matriz[st.fil + 1][st.col + 1] += 1;
			matriz[st.fil - 2][st.col + 2] += 1;
			matriz[st.fil - 1][st.col + 2] += 1;
			matriz[st.fil][st.col + 2] += 1;
			matriz[st.fil + 1][st.col + 2] += 1;
			matriz[st.fil + 2][st.col + 2] += 1;
			matriz[st.fil - 3][st.col + 3] += 1;
			matriz[st.fil - 2][st.col + 3] += 1;
			matriz[st.fil - 1][st.col + 3] += 1;
			matriz[st.fil][st.col + 3] += 1;
			matriz[st.fil + 1][st.col + 3] += 1;
			matriz[st.fil + 2][st.col + 3] += 1;
			matriz[st.fil + 3][st.col + 3] += 1;
			break;

		case sureste:
			matriz[st.fil][st.col + 1] += 1;
			matriz[st.fil + 1][st.col + 1] += 1;
			matriz[st.fil + 1][st.col] += 1;
			matriz[st.fil][st.col + 2] += 1;
			matriz[st.fil + 1][st.col + 2] += 1;
			matriz[st.fil + 2][st.col + 2] += 1;
			matriz[st.fil + 2][st.col + 1] += 1;
			matriz[st.fil + 2][st.col] += 1;
			matriz[st.fil][st.col + 3] += 1;
			matriz[st.fil + 1][st.col + 3] += 1;
			matriz[st.fil + 2][st.col + 3] += 1;
			matriz[st.fil + 3][st.col + 3] += 1;
			matriz[st.fil + 3][st.col + 2] += 1;
			matriz[st.fil + 3][st.col + 1] += 1;
			matriz[st.fil + 3][st.col] += 1;
			break;

		case sur:
			matriz[st.fil + 1][st.col + 1] += 1;
			matriz[st.fil + 1][st.col] += 1;
			matriz[st.fil + 1][st.col - 1] += 1;
			matriz[st.fil + 2][st.col + 2] += 1;
			matriz[st.fil + 2][st.col + 1] += 1;
			matriz[st.fil + 2][st.col] += 1;
			matriz[st.fil + 2][st.col - 1] += 1;
			matriz[st.fil + 2][st.col - 2] += 1;
			matriz[st.fil + 3][st.col + 3] += 1;
			matriz[st.fil + 3][st.col + 2] += 1;
			matriz[st.fil + 3][st.col + 1] += 1;
			matriz[st.fil + 3][st.col] += 1;
			matriz[st.fil + 3][st.col - 1] += 1;
			matriz[st.fil + 3][st.col - 2] += 1;
			matriz[st.fil + 3][st.col - 3] += 1;
			break;

		case suroeste:
			matriz[st.fil + 1][st.col] += 1;
			matriz[st.fil + 1][st.col - 1] += 1;
			matriz[st.fil][st.col - 1] += 1;
			matriz[st.fil + 2][st.col] += 1;
			matriz[st.fil + 2][st.col - 1] += 1;
			matriz[st.fil + 2][st.col - 2] += 1;
			matriz[st.fil + 1][st.col - 2] += 1;
			matriz[st.fil][st.col - 2] += 1;
			matriz[st.fil + 3][st.col] += 1;
			matriz[st.fil + 3][st.col - 1] += 1;
			matriz[st.fil + 3][st.col - 2] += 1;
			matriz[st.fil + 3][st.col - 3] += 1;
			matriz[st.fil + 2][st.col - 3] += 1;
			matriz[st.fil + 1][st.col - 3] += 1;
			matriz[st.fil][st.col - 3] += 1;
			break;

		case oeste:
			matriz[st.fil + 1][st.col - 1] += 1;
			matriz[st.fil][st.col - 1] += 1;
			matriz[st.fil - 1][st.col - 1] += 1;
			matriz[st.fil + 2][st.col - 2] += 1;
			matriz[st.fil + 1][st.col - 2] += 1;
			matriz[st.fil][st.col - 2] += 1;
			matriz[st.fil - 1][st.col - 2] += 1;
			matriz[st.fil - 2][st.col - 2] += 1;
			matriz[st.fil + 3][st.col - 3] += 1;
			matriz[st.fil + 2][st.col - 3] += 1;
			matriz[st.fil + 1][st.col - 3] += 1;
			matriz[st.fil][st.col - 3] += 1;
			matriz[st.fil - 1][st.col - 3] += 1;
			matriz[st.fil - 2][st.col - 3] += 1;
			matriz[st.fil - 3][st.col - 3] += 1;
			break;

		case noroeste:
			matriz[st.fil][st.col - 1] += 1;
			matriz[st.fil - 1][st.col - 1] += 1;
			matriz[st.fil - 1][st.col] += 1;
			matriz[st.fil][st.col - 2] += 1;
			matriz[st.fil - 1][st.col - 2] += 1;
			matriz[st.fil - 2][st.col - 2] += 1;
			matriz[st.fil - 2][st.col - 1] += 1;
			matriz[st.fil - 2][st.col] += 1;
			matriz[st.fil][st.col - 3] += 1;
			matriz[st.fil - 1][st.col - 3] += 1;
			matriz[st.fil - 2][st.col - 3] += 1;
			matriz[st.fil - 3][st.col - 3] += 1;
			matriz[st.fil - 3][st.col - 2] += 1;
			matriz[st.fil - 3][st.col - 1] += 1;
			matriz[st.fil - 3][st.col] += 1;
			break;
		}
	}
	else
	{
		switch (st.brujula)
		{
		case norte:
			matriz[st.fil - 1][st.col - 1] += 1;
			matriz[st.fil - 1][st.col] += 1;
			matriz[st.fil - 1][st.col + 1] += 1;
			matriz[st.fil - 2][st.col - 2] += 1;
			matriz[st.fil - 2][st.col - 1] += 1;
			matriz[st.fil - 2][st.col + 1] += 1;
			matriz[st.fil - 2][st.col + 2] += 1;
			matriz[st.fil - 3][st.col - 3] += 1;
			matriz[st.fil - 3][st.col - 2] += 1;
			matriz[st.fil - 3][st.col + 2] += 1;
			matriz[st.fil - 3][st.col + 3] += 1;
			break;

		case noreste:
			matriz[st.fil - 1][st.col] += 1;
			matriz[st.fil - 1][st.col + 1] += 1;
			matriz[st.fil][st.col + 1] += 1;
			matriz[st.fil - 2][st.col] += 1;
			matriz[st.fil - 2][st.col + 1] += 1;
			matriz[st.fil - 1][st.col + 2] += 1;
			matriz[st.fil][st.col + 2] += 1;
			matriz[st.fil - 3][st.col] += 1;
			matriz[st.fil - 3][st.col + 1] += 1;
			matriz[st.fil - 1][st.col + 3] += 1;
			matriz[st.fil][st.col + 3] += 1;
			break;

		case este:
			matriz[st.fil - 1][st.col + 1] += 1;
			matriz[st.fil][st.col + 1] += 1;
			matriz[st.fil + 1][st.col + 1] += 1;
			;
			matriz[st.fil - 2][st.col + 2] += 1;
			matriz[st.fil - 1][st.col + 2] += 1;
			matriz[st.fil + 1][st.col + 2] += 1;
			matriz[st.fil + 2][st.col + 2] += 1;
			matriz[st.fil - 3][st.col + 3] += 1;
			matriz[st.fil - 2][st.col + 3] += 1;
			matriz[st.fil + 2][st.col + 3] += 1;
			matriz[st.fil + 3][st.col + 3] += 1;
			break;

		case sureste:
			matriz[st.fil][st.col + 1] += 1;
			matriz[st.fil + 1][st.col + 1] += 1;
			matriz[st.fil + 1][st.col] += 1;
			matriz[st.fil][st.col + 2] += 1;
			matriz[st.fil + 1][st.col + 2] += 1;
			matriz[st.fil + 2][st.col + 1] += 1;
			matriz[st.fil + 2][st.col] += 1;
			matriz[st.fil][st.col + 3] += 1;
			matriz[st.fil + 1][st.col + 3] += 1;
			matriz[st.fil + 3][st.col + 1] += 1;
			matriz[st.fil + 3][st.col] += 1;
			break;

		case sur:
			matriz[st.fil + 1][st.col + 1] += 1;
			matriz[st.fil + 1][st.col] += 1;
			matriz[st.fil + 1][st.col - 1] += 1;
			matriz[st.fil + 2][st.col + 2] += 1;
			matriz[st.fil + 2][st.col + 1] += 1;
			matriz[st.fil + 2][st.col - 1] += 1;
			matriz[st.fil + 2][st.col - 2] += 1;
			matriz[st.fil + 3][st.col + 3] += 1;
			matriz[st.fil + 3][st.col + 2] += 1;
			matriz[st.fil + 3][st.col - 2] += 1;
			matriz[st.fil + 3][st.col - 3] += 1;
			break;

		case suroeste:
			matriz[st.fil + 1][st.col] += 1;
			matriz[st.fil + 1][st.col - 1] += 1;
			matriz[st.fil][st.col - 1] += 1;
			matriz[st.fil + 2][st.col] += 1;
			matriz[st.fil + 2][st.col - 1] += 1;
			matriz[st.fil + 1][st.col - 2] += 1;
			matriz[st.fil][st.col - 2] += 1;
			matriz[st.fil + 3][st.col] += 1;
			matriz[st.fil + 3][st.col - 1] += 1;
			matriz[st.fil + 1][st.col - 3] += 1;
			matriz[st.fil][st.col - 3] += 1;
			break;

		case oeste:
			matriz[st.fil + 1][st.col - 1] += 1;
			matriz[st.fil][st.col - 1] += 1;
			matriz[st.fil - 1][st.col - 1] += 1;
			matriz[st.fil + 2][st.col - 2] += 1;
			matriz[st.fil + 1][st.col - 2] += 1;
			matriz[st.fil - 1][st.col - 2] += 1;
			matriz[st.fil - 2][st.col - 2] += 1;
			matriz[st.fil + 3][st.col - 3] += 1;
			matriz[st.fil + 2][st.col - 3] += 1;
			matriz[st.fil - 2][st.col - 3] += 1;
			matriz[st.fil - 3][st.col - 3] += 1;
			break;

		case noroeste:
			matriz[st.fil][st.col - 1] += 1;
			matriz[st.fil - 1][st.col - 1] += 1;
			matriz[st.fil - 1][st.col] += 1;
			matriz[st.fil][st.col - 2] += 1;
			matriz[st.fil - 1][st.col - 2] += 1;
			matriz[st.fil - 2][st.col - 1] += 1;
			matriz[st.fil - 2][st.col] += 1;
			matriz[st.fil][st.col - 3] += 1;
			matriz[st.fil - 1][st.col - 3] += 1;
			matriz[st.fil - 3][st.col - 1] += 1;
			matriz[st.fil - 3][st.col] += 1;
			break;
		}
	}
}
void ComportamientoJugador::PonerTerrenoMatrizNoSituado(const vector<unsigned char> &terreno, const state &st, vector<vector<unsigned char>> &matriz, Orientacion pos, Sensores sensor)
{
	switch (last_action)
	{

	case actWALK:
		if (sensor.colision == false)
		{
			switch (current_state.brujula)
			{
			case norte:
				fil--;
				break;
			case sur:
				fil++;
				break;
			case este:
				col++;
				break;
			case oeste:
				col--;
				break;
			case noreste:
				fil--;
				col++;
				break;
			case noroeste:
				fil--;
				col--;
				break;
			case sureste:
				fil++;
				col++;
				break;
			case suroeste:
				fil++;
				col--;
				break;
			}
		}
		break;

	case actRUN:
		if (sensor.colision == false)
		{
			switch (current_state.brujula)
			{
			case norte:
				fil -= 2;
				break;
			case sur:
				fil += 2;
				break;
			case este:
				col += 2;
				break;
			case oeste:
				col -= 2;
				break;
			case noreste:
				fil -= 2;
				col += 2;
				break;
			case noroeste:
				fil -= 2;
				col -= 2;
				break;
			case sureste:
				fil += 2;
				col += 2;
				break;
			case suroeste:
				fil += 2;
				col -= 2;
				break;
			}
		}
		break;
	case actTURN_SR:
		a = current_state.brujula;
		a = (a + 1) % 8;
		a_norte = (a + 1) % 8;
		a_sur = (a + 1) % 8;
		a_este = (a + 1) % 8;
		a_oeste = (a + 1) % 8;
		current_state.brujula = static_cast<Orientacion>(a);
		ini_norte = static_cast<Orientacion>(a_norte);
		ini_sur = static_cast<Orientacion>(a_sur);
		ini_este = static_cast<Orientacion>(a_este);
		ini_oeste = static_cast<Orientacion>(a_oeste);
		break;
	case actTURN_L:
		a = current_state.brujula;
		a = (a + 6) % 8;
		a_norte = (a + 6) % 8;
		a_sur = (a + 6) % 8;
		a_este = (a + 6) % 8;
		a_oeste = (a + 6) % 8;
		current_state.brujula = static_cast<Orientacion>(a);
		ini_norte = static_cast<Orientacion>(a_norte);
		ini_sur = static_cast<Orientacion>(a_sur);
		ini_este = static_cast<Orientacion>(a_este);
		ini_oeste = static_cast<Orientacion>(a_oeste);
		break;
	}

	if (fil < 0 || fil >= matriz.size() || col < 0 || col >= matriz[0].size())
	{
		cout << "Error: Índices fuera de los límites de la matriz." << endl;
		return;
	}
	matriz[fil][col] = terreno[0];
	if (sensor.nivel != 3)
	{
		switch (st.brujula)
		{
		case norte:
			matriz[fil - 1][col - 1] = terreno[1];
			matriz[fil - 1][st.col] = terreno[2];
			matriz[fil - 1][col + 1] = terreno[3];
			matriz[fil - 2][col - 2] = terreno[4];
			matriz[fil - 2][col - 1] = terreno[5];
			matriz[fil - 2][col] = terreno[6];
			matriz[fil - 2][col + 1] = terreno[7];
			matriz[fil - 2][col + 2] = terreno[8];
			matriz[fil - 3][col - 3] = terreno[9];
			matriz[fil - 3][col - 2] = terreno[10];
			matriz[fil - 3][col - 1] = terreno[11];
			matriz[fil - 3][col] = terreno[12];
			matriz[fil - 3][col + 1] = terreno[13];
			matriz[fil - 3][col + 2] = terreno[14];
			matriz[fil - 3][col + 3] = terreno[15];
			break;

		case noreste:
			matriz[fil - 1][col] = terreno[1];
			matriz[fil - 1][col + 1] = terreno[2];
			matriz[fil][col + 1] = terreno[3];
			matriz[fil - 2][col] = terreno[4];
			matriz[fil - 2][col + 1] = terreno[5];
			matriz[fil - 2][col + 2] = terreno[6];
			matriz[fil - 1][col + 2] = terreno[7];
			matriz[fil][col + 2] = terreno[8];
			matriz[fil - 3][col] = terreno[9];
			matriz[fil - 3][col + 1] = terreno[10];
			matriz[fil - 3][col + 2] = terreno[11];
			matriz[fil - 3][col + 3] = terreno[12];
			matriz[fil - 2][col + 3] = terreno[13];
			matriz[fil - 1][col + 3] = terreno[14];
			matriz[fil][col + 3] = terreno[15];
			break;

		case este:
			matriz[fil - 1][col + 1] = terreno[1];
			matriz[fil][col + 1] = terreno[2];
			matriz[fil + 1][col + 1] = terreno[3];
			matriz[fil - 2][col + 2] = terreno[4];
			matriz[fil - 1][col + 2] = terreno[5];
			matriz[fil][col + 2] = terreno[6];
			matriz[fil + 1][col + 2] = terreno[7];
			matriz[fil + 2][col + 2] = terreno[8];
			matriz[fil - 3][col + 3] = terreno[9];
			matriz[fil - 2][col + 3] = terreno[10];
			matriz[fil - 1][col + 3] = terreno[11];
			matriz[fil][col + 3] = terreno[12];
			matriz[fil + 1][col + 3] = terreno[13];
			matriz[fil + 2][col + 3] = terreno[14];
			matriz[fil + 3][col + 3] = terreno[15];
			break;

		case sureste:
			matriz[fil][col + 1] = terreno[1];
			matriz[fil + 1][col + 1] = terreno[2];
			matriz[fil + 1][col] = terreno[3];
			matriz[fil][col + 2] = terreno[4];
			matriz[fil + 1][col + 2] = terreno[5];
			matriz[fil + 2][col + 2] = terreno[6];
			matriz[fil + 2][col + 1] = terreno[7];
			matriz[fil + 2][col] = terreno[8];
			matriz[fil][col + 3] = terreno[9];
			matriz[fil + 1][col + 3] = terreno[10];
			matriz[st.fil + 2][col + 3] = terreno[11];
			matriz[fil + 3][col + 3] = terreno[12];
			matriz[fil + 3][col + 2] = terreno[13];
			matriz[fil + 3][col + 1] = terreno[14];
			matriz[fil + 3][col] = terreno[15];
			break;

		case sur:
			matriz[fil + 1][col + 1] = terreno[1];
			matriz[fil + 1][col] = terreno[2];
			matriz[fil + 1][col - 1] = terreno[3];
			matriz[fil + 2][col + 2] = terreno[4];
			matriz[fil + 2][col + 1] = terreno[5];
			matriz[fil + 2][col] = terreno[6];
			matriz[fil + 2][col - 1] = terreno[7];
			matriz[fil + 2][col - 2] = terreno[8];
			matriz[fil + 3][col + 3] = terreno[9];
			matriz[fil + 3][col + 2] = terreno[10];
			matriz[fil + 3][col + 1] = terreno[11];
			matriz[fil + 3][col] = terreno[12];
			matriz[fil + 3][col - 1] = terreno[13];
			matriz[fil + 3][col - 2] = terreno[14];
			matriz[fil + 3][col - 3] = terreno[15];
			break;

		case suroeste:
			matriz[fil + 1][col] = terreno[1];
			matriz[fil + 1][col - 1] = terreno[2];
			matriz[fil][st.col - 1] = terreno[3];
			matriz[fil + 2][col] = terreno[4];
			matriz[fil + 2][col - 1] = terreno[5];
			matriz[fil + 2][col - 2] = terreno[6];
			matriz[fil + 1][col - 2] = terreno[7];
			matriz[fil][col - 2] = terreno[8];
			matriz[fil + 3][col] = terreno[9];
			matriz[fil + 3][col - 1] = terreno[10];
			matriz[fil + 3][col - 2] = terreno[11];
			matriz[fil + 3][col - 3] = terreno[12];
			matriz[fil + 2][col - 3] = terreno[13];
			matriz[fil + 1][col - 3] = terreno[14];
			matriz[fil][col - 3] = terreno[15];
			break;

		case oeste:
			matriz[fil + 1][col - 1] = terreno[1];
			matriz[fil][col - 1] = terreno[2];
			matriz[fil - 1][col - 1] = terreno[3];
			matriz[fil + 2][col - 2] = terreno[4];
			matriz[fil + 1][col - 2] = terreno[5];
			matriz[fil][col - 2] = terreno[6];
			matriz[fil - 1][col - 2] = terreno[7];
			matriz[fil - 2][col - 2] = terreno[8];
			matriz[fil + 3][col - 3] = terreno[9];
			matriz[fil + 2][col - 3] = terreno[10];
			matriz[fil + 1][col - 3] = terreno[11];
			matriz[fil][col - 3] = terreno[12];
			matriz[fil - 1][col - 3] = terreno[13];
			matriz[fil - 2][col - 3] = terreno[14];
			matriz[fil - 3][col - 3] = terreno[15];
			break;

		case noroeste:
			matriz[fil][col - 1] = terreno[1];
			matriz[fil - 1][col - 1] = terreno[2];
			matriz[fil - 1][col] = terreno[3];
			matriz[fil][col - 2] = terreno[4];
			matriz[fil - 1][col - 2] = terreno[5];
			matriz[fil - 2][col - 2] = terreno[6];
			matriz[fil - 2][col - 1] = terreno[7];
			matriz[fil - 2][col] = terreno[8];
			matriz[fil][col - 3] = terreno[9];
			matriz[fil - 1][col - 3] = terreno[10];
			matriz[fil - 2][col - 3] = terreno[11];
			matriz[fil - 3][col - 3] = terreno[12];
			matriz[fil - 3][col - 2] = terreno[13];
			matriz[fil - 3][col - 1] = terreno[14];
			matriz[fil - 3][col] = terreno[15];
			break;
		}
	}
	else
	{
		switch (st.brujula)
		{
		case norte:
			matriz[fil - 1][col - 1] = terreno[1];
			matriz[fil - 1][col] = terreno[2];
			matriz[fil - 1][col + 1] = terreno[3];
			matriz[fil - 2][col - 2] = terreno[4];
			matriz[fil - 2][col - 1] = terreno[5];
			matriz[fil - 2][col + 1] = terreno[7];
			matriz[fil - 2][col + 2] = terreno[8];
			matriz[fil - 3][col - 3] = terreno[9];
			matriz[fil - 3][col - 2] = terreno[10];
			matriz[fil - 3][col + 2] = terreno[14];
			matriz[fil - 3][col + 3] = terreno[15];
			break;

		case noreste:
			matriz[fil - 1][col] = terreno[1];
			matriz[fil - 1][col + 1] = terreno[2];
			matriz[fil][col + 1] = terreno[3];
			matriz[fil - 2][col] = terreno[4];
			matriz[fil - 2][col + 1] = terreno[5];
			matriz[fil - 1][col + 2] = terreno[7];
			matriz[fil][col + 2] = terreno[8];
			matriz[fil - 3][col] = terreno[9];
			matriz[fil - 3][col + 1] = terreno[10];
			matriz[fil - 1][col + 3] = terreno[14];
			matriz[fil][col + 3] = terreno[15];
			break;

		case este:
			matriz[fil - 1][col + 1] = terreno[1];
			matriz[fil][col + 1] = terreno[2];
			matriz[fil + 1][col + 1] = terreno[3];
			matriz[fil - 2][col + 2] = terreno[4];
			matriz[fil - 1][col + 2] = terreno[5];
			matriz[fil + 1][col + 2] = terreno[7];
			matriz[fil + 2][col + 2] = terreno[8];
			matriz[fil - 3][col + 3] = terreno[9];
			matriz[fil - 2][col + 3] = terreno[10];
			matriz[fil + 2][col + 3] = terreno[14];
			matriz[fil + 3][col + 3] = terreno[15];
			break;

		case sureste:
			matriz[fil][col + 1] = terreno[1];
			matriz[fil + 1][col + 1] = terreno[2];
			matriz[fil + 1][col] = terreno[3];
			matriz[fil][col + 2] = terreno[4];
			matriz[fil + 1][col + 2] = terreno[5];
			matriz[fil + 2][col + 1] = terreno[7];
			matriz[fil + 2][col] = terreno[8];
			matriz[fil][col + 3] = terreno[9];
			matriz[fil + 1][col + 3] = terreno[10];
			matriz[fil + 3][col + 1] = terreno[14];
			matriz[fil + 3][col] = terreno[15];
			break;

		case sur:
			matriz[fil + 1][col + 1] = terreno[1];
			matriz[fil + 1][col] = terreno[2];
			matriz[fil + 1][col - 1] = terreno[3];
			matriz[fil + 2][col + 2] = terreno[4];
			matriz[fil + 2][col + 1] = terreno[5];
			matriz[fil + 2][col - 1] = terreno[7];
			matriz[fil + 2][col - 2] = terreno[8];
			matriz[fil + 3][col + 3] = terreno[9];
			matriz[fil + 3][col + 2] = terreno[10];
			matriz[fil + 3][col - 2] = terreno[14];
			matriz[fil + 3][col - 3] = terreno[15];
			break;

		case suroeste:
			matriz[fil + 1][col] = terreno[1];
			matriz[fil + 1][col - 1] = terreno[2];
			matriz[fil][col - 1] = terreno[3];
			matriz[fil + 2][col] = terreno[4];
			matriz[fil + 2][col - 1] = terreno[5];
			matriz[fil + 1][col - 2] = terreno[7];
			matriz[fil][col - 2] = terreno[8];
			matriz[fil + 3][col] = terreno[9];
			matriz[fil + 3][col - 1] = terreno[10];
			matriz[fil + 1][col - 3] = terreno[14];
			matriz[fil][col - 3] = terreno[15];
			break;

		case oeste:
			matriz[fil + 1][col - 1] = terreno[1];
			matriz[fil][col - 1] = terreno[2];
			matriz[fil - 1][col - 1] = terreno[3];
			matriz[fil + 2][col - 2] = terreno[4];
			matriz[fil + 1][col - 2] = terreno[5];
			matriz[fil - 1][col - 2] = terreno[7];
			matriz[fil - 2][col - 2] = terreno[8];
			matriz[fil + 3][col - 3] = terreno[9];
			matriz[fil + 2][col - 3] = terreno[10];
			matriz[fil - 2][col - 3] = terreno[14];
			matriz[fil - 3][col - 3] = terreno[15];
			break;

		case noroeste:
			matriz[fil][col - 1] = terreno[1];
			matriz[fil - 1][col - 1] = terreno[2];
			matriz[fil - 1][col] = terreno[3];
			matriz[fil][col - 2] = terreno[4];
			matriz[fil - 1][col - 2] = terreno[5];
			matriz[fil - 2][col - 1] = terreno[7];
			matriz[fil - 2][col] = terreno[8];
			matriz[fil][col - 3] = terreno[9];
			matriz[fil - 1][col - 3] = terreno[10];
			matriz[fil - 3][col - 1] = terreno[14];
			matriz[fil - 3][col] = terreno[15];
			break;
		}
	}
}
/* Crear matriz
Tenemos que crear una otra matriz para cunado no estamos bien situados que empiece a almacenar en la casilla 99 99
la matriz la hacemos de 200 200 y cuando estemos bien posicionados a partir de lo que hemos visto en esta matriz pegamos en el mapa Resulatdo
*/