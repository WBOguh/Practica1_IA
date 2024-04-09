#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include <cmath>
#include <limits>

using namespace std;

Action ComportamientoJugador::think(Sensores sensores)
{

	Action accion = actIDLE;

	/*// Mostrar el valor de los sensores
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
*/
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
	if (last_action == actTURN_L)
		cont_giros++;
	else if (last_action == actTURN_SR)
		cont_giros = 0;
	if (last_action == actWALK)
		cont_pasos = 0;
	if (sensores.bateria < 3000)
		busco_bateria = true;
	else
		busco_bateria = false;
	if (sensores.reset == true)
	{
		last_action = actIDLE;
		current_state.col = 99;
		current_state.fil = 99;
		if (sensores.nivel != 0 and sensores.nivel != 3)
			current_state.brujula = norte;
		tengo_bikini = false;
		tengo_zapatilllas = false;
		if (sensores.nivel != 0)
		{
			bien_situado = false;
		}

		for (int i = 0; i < mapaNoSituado.size(); i++)
		{
			for (int j = 0; j < mapaNoSituado[0].size(); j++)
			{
				mapaNoSituado[i][j] = '?';
			}
		}
		col = 99;
		fil = 99;
		ini_norte = norte;
		ini_este = este;
		ini_oeste = oeste;
		ini_sur = sur;
	}

	if (sensores.nivel == 0)
	{
		current_state.fil = sensores.posF;
		current_state.col = sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado = true;
	}
	else if ((sensores.nivel == 1 or sensores.nivel == 2) && norte_una_vez)
	{
		current_state.brujula = norte;
		norte_una_vez = false;
	}
	if (sensores.terreno[0] == 'G' and !bien_situado)
	{
		PonerTerrenoMatrizNoSituado(sensores.terreno, current_state, mapaNoSituado, current_state.brujula, sensores);
		current_state.fil = sensores.posF;
		current_state.col = sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado = true;
		// cout << "Bien posicionado" << endl;
		if (sensores.nivel != 3)
			PonValoresNoPosicionadoAVerdaderos(mapaResultado, mapaNoSituado, current_state.fil, current_state.col, sensores, current_state.brujula);
	}
	else if (sensores.terreno[0] == 'B' and !tengo_bikini)
		tengo_bikini = true;
	else if (sensores.terreno[0] == 'D' and !tengo_zapatilllas)
		tengo_zapatilllas = true;

	if (!bien_situado)
	{
		PonerTerrenoMatrizNoSituado(sensores.terreno, current_state, mapaNoSituado, current_state.brujula, sensores);
	}

	if (bien_situado)
	{
		ponerpasosenvector(pasos, current_state, mapaPasos, current_state.brujula, sensores);
		PonerTerrenoMatriz(sensores.terreno, current_state, mapaResultado, current_state.brujula, sensores);
		PonerCantidadEnAuxiliar(current_state, mapaPasos, current_state.brujula, sensores);
	}
	pos = HayCasilaEspecialNecesariaEnVista(sensores.terreno, sensores);

	if (hay_algo and pos != 0 and sensores.terreno[1] != 'M' and sensores.terreno[1] != 'P' and sensores.terreno[2] != 'M' and sensores.terreno[2] != 'P' and sensores.terreno[3] != 'M' and sensores.terreno[3] != 'P')
	{
		accion = IrCasillaEnVista(pos, sensores.terreno, sensores);
		// cout << "Casilla en vista dirigiendose" << endl;
	}
	else if (bien_situado)
	{
		for (int i = 0; i < 16; i++)
		{
			if (sensores.terreno[i] == 'M' or sensores.terreno[i] == 'P')
				obstaculos = true;
		}
		pos = BuscarCasillaMenosVista(pasos, 1, 15);
		accion = HayEntidadesEnVista(sensores);
		if (accion == actIDLE)
		{
			if (obstaculos)
			{
				if (sensores.terreno[1] != 'M' and sensores.terreno[1] != 'P' and sensores.terreno[2] != 'M' and sensores.terreno[2] != 'P' and sensores.terreno[3] != 'M' and sensores.terreno[3] != 'P')
				{
					pos = BuscarCasillaMenosVista(pasos, 1, 3);
					obstaculos = false;
				}
			}
			if (!obstaculos)
				accion = IrCasillaEnVista(pos, sensores.terreno, sensores);
			if (imposible)
				accion = movimientoGeneral(sensores, current_state);
			/*else if (accion == actWALK and sensores.terreno[2] == 'B' and !tengo_zapatilllas)
				accion = movimientoGeneral(sensores, current_state);*/
			else if (accion == actWALK and sensores.terreno[2] == 'A' and !tengo_bikini)
				accion = movimientoGeneral(sensores, current_state);
			else if (obstaculos)
				accion = movimientoGeneral(sensores, current_state);
		}
		obstaculos = false;
	}
	else
	{
		accion = movimientoGeneral(sensores, current_state);
	}
	if (imposible)
		accion = movimientoGeneral(sensores, current_state);
	if (busco_bateria and sensores.terreno[0] == 'X')
	{
		accion = actIDLE;
	}
	if (last_action == actTURN_SR && girar_derecha)
	{
		accion = actTURN_SR;
		girar_derecha = false;
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

	if (hay_casilla_recuperacion and busco_bateria)
	{
		pos = pos_recuperacion;
	}
	else if (hay_casilla_posicion and !bien_situado)
	{
		pos = pos_posicion;
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
	// cout << "Posicion: " << pos << endl;
	return pos;
}
Action ComportamientoJugador::IrCasillaEnVista(int pos, const vector<unsigned char> &terreno, Sensores sensor)
{
	imposible = false;
	Action accion;
	if (pos == 1 or pos == 4 or pos == 9)
		accion = actTURN_L;
	else if (pos == 3 or pos == 8 or pos == 15)
		accion = actTURN_SR;
	else if ((terreno[2] != 'M' and terreno[2] != 'P') and sensor.agentes[2] == '_')
		accion = actWALK;
	else if ((terreno[2] != 'M' and terreno[2] != 'P') and sensor.agentes[2] != 'a')
		accion = actIDLE;
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
		// cout << "Error: Índices fuera de los límites de la matriz." << endl;
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
		// cout << "Error: Índices fuera de los límites de la matriz." << endl;
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
			switch (ini_norte)
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
			switch (ini_norte)
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
		b = ini_norte;
		b = (b + 1) % 8;
		ini_norte = static_cast<Orientacion>(b);
		c = ini_este;
		c = (c + 1) % 8;
		ini_este = static_cast<Orientacion>(c);
		d = ini_sur;
		d = (d + 1) % 8;
		ini_sur = static_cast<Orientacion>(d);
		e = ini_oeste;
		e = (e + 1) % 8;
		ini_oeste = static_cast<Orientacion>(e);
		break;
	case actTURN_L:
		b = ini_norte;
		b = (b - 2 + 8) % 8;
		ini_norte = static_cast<Orientacion>(b);
		c = ini_este;
		c = (c - 2 + 8) % 8;
		ini_este = static_cast<Orientacion>(c);
		d = ini_sur;
		d = (d - 2 + 8) % 8;
		ini_sur = static_cast<Orientacion>(d);
		e = ini_oeste;
		e = (e - 2 + 8) % 8;
		ini_oeste = static_cast<Orientacion>(e);
		break;
	}

	if (fil < 0 || fil >= matriz.size() || col < 0 || col >= matriz[0].size())
	{
		// cout << "Error: Índices fuera de los límites de la matriz." << endl;
		return;
	}
	matriz[fil][col] = terreno[0];
	fil_pos = fil;
	col_pos = col;
	// cout << "Escribiendo en matriz auxiliar a partir de:" << fil << " " << col << " " << terreno[0] << endl;
	if (sensor.nivel != 3)
	{
		switch (ini_norte)
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
		switch (pos)
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
void ComportamientoJugador::PonValoresNoPosicionadoAVerdaderos(vector<vector<unsigned char>> &matriz_pequeña, vector<vector<unsigned char>> &matriz_grande, int fila, int columna, Sensores sensor, Orientacion situado)
{
	// cout << situado << " " << ini_norte << " " << ini_este << " " << ini_sur << " " << ini_oeste << endl;
	int fila_aux = fil_pos, col_aux = col_pos;
	// cout << fil_pos << " " << col_pos << endl;
	// cout << "Fila: " << fila << " Columna: " << columna << endl;
	if (situado == ini_norte)
	{
		// cout << "Norte" << endl;
		for (int i = 0; i < matriz_pequeña.size(); ++i)
		{
			for (int j = 0; j < matriz_pequeña[0].size(); ++j)
			{
				if (matriz_pequeña[i][j] == '?' and matriz_grande[i + (fil_pos - fila)][j + (col_pos - columna)] != '?')
				{
					matriz_pequeña[i][j] = matriz_grande[i + (fil_pos - fila)][j + (col_pos - columna)];
					mapaPasos[i][j]++;
				}
			}
		}
		for (int i = 0; i < matriz_grande.size(); ++i)
		{
			for (int j = 0; j < matriz_grande[0].size(); ++j)
			{
				matriz_grande[i][j] = '?';
			}
		}
	}
	else if (situado == ini_este)
	{
		girar_matriz_derecha(matriz_grande);
		fil_pos = col_pos;
		col_pos = matriz_grande[0].size() - fila_aux - 1;

		for (int i = 0; i < matriz_pequeña.size(); ++i)
		{
			for (int j = 0; j < matriz_pequeña[0].size(); ++j)
			{

				if (matriz_pequeña[i][j] == '?' and matriz_grande[i + (fil_pos - fila)][j + (col_pos - columna)] != '?')
				{
					matriz_pequeña[i][j] = matriz_grande[i + (fil_pos - fila)][j + (col_pos - columna)];
					mapaPasos[i][j]++;
				}
			}
		}
		for (int i = 0; i < matriz_grande.size(); ++i)
		{
			for (int j = 0; j < matriz_grande[0].size(); ++j)
			{
				matriz_grande[i][j] = '?';
			}
		}
	}
	else if (situado == ini_sur)
	{
		for (int i = 0; i < 2; i++)
			girar_matriz_derecha(matriz_grande);
		fil_pos = matriz_grande.size() - fila_aux - 1;
		col_pos = matriz_grande[0].size() - col_aux - 1;
		for (int i = 0; i < matriz_pequeña.size(); ++i)
		{
			for (int j = 0; j < matriz_pequeña[0].size(); ++j)
			{
				if (matriz_pequeña[i][j] == '?' and matriz_grande[i + (fil_pos - fila)][j + (col_pos - columna)] != '?')
				{
					matriz_pequeña[i][j] = matriz_grande[i + (fil_pos - fila)][j + (col_pos - columna)];
					mapaPasos[i][j]++;
				}
			}
		}

		for (int i = 0; i < matriz_grande.size(); ++i)
		{
			for (int j = 0; j < matriz_grande[0].size(); ++j)
			{
				matriz_grande[i][j] = '?';
			}
		}
	}
	else if (situado == ini_oeste)
	{
		for (int i = 0; i < 3; i++)
			girar_matriz_derecha(matriz_grande);

		fil_pos = matriz_grande[0].size() - col_aux - 1;
		col_pos = fila_aux;
		for (int i = 0; i < matriz_pequeña.size(); ++i)
		{
			for (int j = 0; j < matriz_pequeña[0].size(); ++j)
			{
				if (matriz_pequeña[i][j] == '?' and matriz_grande[i + (fil_pos - fila)][j + (col_pos - columna)] != '?')
				{
					matriz_pequeña[i][j] = matriz_grande[i + (fil_pos - fila)][j + (col_pos - columna)];
					mapaPasos[i][j]++;
				}
			}
		}

		for (int i = 0; i < matriz_grande.size(); ++i)
		{
			for (int j = 0; j < matriz_grande[0].size(); ++j)
			{
				matriz_grande[i][j] = '?';
			}
		}
	}
}
void ComportamientoJugador::girar_matriz_derecha(vector<vector<unsigned char>> &matriz)
{
	vector<vector<unsigned char>> matriz_aux(matriz.size(), vector<unsigned char>(matriz[0].size(), '0'));
	for (int i = 0; i < matriz.size(); ++i)
	{
		for (int j = 0; j < matriz[0].size(); ++j)
		{
			matriz_aux[j][matriz.size() - i - 1] = matriz[i][j];
		}
	}
	matriz = matriz_aux;
}
Action ComportamientoJugador::movimientoGeneral(Sensores sensor, state current_state)
{
	Action accion = actIDLE;
	int next_pos = -1;

	accion = HayEntidadesEnVista(sensor);
	if (accion == actIDLE)
	{
		if (sensor.terreno[2] == 'T' or sensor.terreno[2] == 'S' or sensor.terreno[2] == 'G' and sensor.agentes[2] == '_')
		{
			accion = actWALK;
			if (sensor.terreno[3] != 'M' and sensor.terreno[3] != 'P' and sensor.terreno[7] == 'M' and (sensor.terreno[13] == 'M' or sensor.terreno[13] == 'P'))
			{
				accion = actTURN_SR;
			}
			// cout << "Nada delante o salida muro" << endl;
		}
		else if ((sensor.terreno[0] == 'A' and !tengo_bikini) or (sensor.terreno[0] == 'B' and !tengo_zapatilllas))
		{
			accion = Atrapado(sensor.terreno, sensor);
			// cout << "Atrapado" << endl;
		}
		else if (sensor.terreno[2] == 'M' or sensor.terreno[1] == 'M' or sensor.terreno[3] == 'M' or sensor.terreno[5] == 'M' or sensor.terreno[7] == 'M')
		{
			accion = SeguirMuro(sensor);
			// cout << "Muro" << endl;
		}
		else if (sensor.terreno[2] == 'P')
		{
			accion = actTURN_L;
			if (rand() % 2 == 0)
			{
				accion = actTURN_SR;
				girar_derecha = true;
			}
			// cout << "Precipicio" << endl;
		}
		else if (sensor.terreno[2] == 'A' and !tengo_bikini)
		{
			accion = Atrapado(sensor.terreno, sensor);
			// cout << "Atrapado enfrente agua" << endl;
		}
		else if (sensor.terreno[2] == 'B' and !tengo_zapatilllas)
		{
			accion = Atrapado(sensor.terreno, sensor);
			// cout << "Atrapado enfrente bosque" << endl;
		}
		else
		{
			accion = actWALK;
			// cout << "Andar" << endl;
		}
		if ((cont_pasos > mapaResultado.size() / 3) and accion == actWALK)
			if (rand() % 2 == 0)
				accion = actTURN_L;
		if (cont_pasos >= 10 and accion == actWALK)
		{
			if (rand() % 4 == 0)
				accion = actTURN_L;
			else if (rand() % 4 == 0)
				accion = actTURN_SR;
		}
		if (accion == actWALK)
		{
			if (rand() % 10 == 0)
				accion = actTURN_L;
			else if (rand() % 10 == 0)
				accion = actTURN_SR;
		}

		if (accion == actWALK)
			cont_pasos++;
	}
	return accion;
}
Action ComportamientoJugador::HayEntidadesEnVista(Sensores sensor)
{
	Action accion;
	pos_lobo = 99;
	pos_aldeano = 99;
	for (int i = 15; i >= 1; --i)
	{
		if (sensor.agentes[i] == 'l')
			pos_lobo = i;
		else if (sensor.agentes[i] == 'a')
			pos_aldeano = i;
	}

	if (pos_lobo != 99)
	{
		if (pos_lobo == 1 or pos_lobo == 4 or pos_lobo == 5 or pos_lobo == 9 or pos_lobo == 10 or pos_lobo == 11)
			accion = actTURN_SR;
		else
			accion = actTURN_L;
	}
	else if (pos_aldeano != 99)
	{
		if (pos_aldeano == 1 or pos_aldeano == 4 or pos_aldeano == 5 or pos_aldeano == 9 or pos_aldeano == 10 or pos_aldeano == 11)
			accion = actTURN_SR;
		else
			accion = actTURN_L;
	}
	else
		accion = actIDLE;
	return accion;
}
Action ComportamientoJugador::Atrapado(const vector<unsigned char> &terreno, Sensores sensor)
{
	Action accion;
	int pos = 99;

	if (terreno[2] != 'M' and terreno[2] != 'P' and terreno[2] != 'A' and terreno[2] != 'B' and sensor.agentes[2] == '_')
		pos = 2;
	else
	{
		for (int i = 15; i <= 1; --i)
		{
			if (terreno[0] == 'A')
			{
				if (terreno[i] != 'A' and terreno[i] != 'P' and terreno[i] != 'M')
					pos = i;
			}
			else
			{
				if (terreno[i] != 'B' and terreno[i] != 'P' and terreno[i] != 'M' and terreno[i] != 'A')
					pos = i;
			}
		}
	}
	// cout << pos << endl;
	if (pos != 99)
		accion = IrCasillaEnVista(pos, terreno, sensor);
	if (imposible)
	{
		accion = actTURN_L;

		if (rand() % 3 == 0)
		{
			accion = actTURN_SR;
			girar_derecha = true;
		}
	}
	else if (cont_giros >= 4)
		if (terreno[2] != 'P' and terreno[2] != 'M')
			accion = actWALK;
		else
			accion = actTURN_L;
	else
		accion = actTURN_L;

	return accion;
}
Action ComportamientoJugador::SeguirMuro(Sensores sensor)
{
	Action accion;
	if ((sensor.terreno[1] == 'M' or sensor.terreno[1] == 'P') and sensor.terreno[2] == 'M' and (sensor.terreno[3] == 'M' or sensor.terreno[3] == 'P'))
	{
		accion = actTURN_L;

		if (rand() % 3 == 0)
		{
			accion = actTURN_SR;
			girar_derecha = true;
		}
	}
	else if (sensor.terreno[1] == 'M' and (sensor.terreno[2] != 'P' and sensor.terreno[2] != 'M') and sensor.terreno[3] == 'M' and sensor.agentes[2] == '_')
		accion = actWALK;
	else if (sensor.terreno[7] == 'M' and sensor.terreno[3] != 'M' and sensor.terreno[3] != 'P')
		accion = actTURN_SR;
	else if (sensor.terreno[5] == 'M' and sensor.terreno[1] != 'M' and sensor.terreno[1] != 'P')
		accion = actTURN_L;
	else if (sensor.terreno[1] == 'M' and sensor.terreno[2] == 'M' and sensor.terreno[3] != 'M' and sensor.terreno[3] != 'P' and sensor.agentes[3] == '_')
		accion = actTURN_SR;
	else if (sensor.terreno[3] == 'M' and sensor.terreno[2] == 'M' and sensor.terreno[1] != 'M' and sensor.terreno[1] != 'P' and sensor.agentes[1] == '_')
		accion = actTURN_L;
	else if (((sensor.terreno[1] == 'M' or sensor.terreno[1] == 'P') or (sensor.terreno[3] == 'M' or sensor.terreno[3] == 'P')) and (sensor.terreno[2] != 'M' or sensor.terreno[1] != 'P' and sensor.agentes[2] == '_'))
		accion = actWALK;
	else if (sensor.terreno[2] == 'M' and sensor.terreno[1] != 'M' and sensor.terreno[1] != 'P' and sensor.terreno[3] != 'M' and sensor.terreno[3] != 'P')
	{
		if (sensor.agentes[3] == '_')
			accion = actTURN_SR;
		else
			accion = actTURN_L;
	}
	else if (sensor.terreno[3] == 'M' and sensor.terreno[2] == 'M' and (sensor.terreno[1] == 'M' or sensor.terreno[1] == 'P'))
	{
		accion = actTURN_SR;
		girar_derecha = true;
	}
	return accion;
}
void ComportamientoJugador::ponerpasosenvector(vector<int> &terreno, const state &st, const vector<vector<int>> &matriz, Orientacion pos, Sensores sensor)
{
	if (st.fil < 0 || st.fil >= matriz.size() || st.col < 0 || st.col >= matriz[0].size())
	{
		// cout << "Error: Índices fuera de los límites de la matriz." << endl;
		return;
	}
	terreno[0] = matriz[st.fil][st.col];
	if (sensor.nivel != 3)
	{
		switch (st.brujula)
		{
		case norte:
			terreno[1] = matriz[st.fil - 1][st.col - 1];
			terreno[2] = matriz[st.fil - 1][st.col];
			terreno[3] = matriz[st.fil - 1][st.col + 1];
			terreno[4] = matriz[st.fil - 2][st.col - 2];
			terreno[5] = matriz[st.fil - 2][st.col - 1];
			terreno[6] = matriz[st.fil - 2][st.col];
			terreno[7] = matriz[st.fil - 2][st.col + 1];
			terreno[8] = matriz[st.fil - 2][st.col + 2];
			terreno[9] = matriz[st.fil - 3][st.col - 3];
			terreno[10] = matriz[st.fil - 3][st.col - 2];
			terreno[11] = matriz[st.fil - 3][st.col - 1];
			terreno[12] = matriz[st.fil - 3][st.col];
			terreno[13] = matriz[st.fil - 3][st.col + 1];
			terreno[14] = matriz[st.fil - 3][st.col + 2];
			terreno[15] = matriz[st.fil - 3][st.col + 3];
			break;

		case noreste:
			terreno[1] = matriz[st.fil - 1][st.col];
			terreno[2] = matriz[st.fil - 1][st.col + 1];
			terreno[3] = matriz[st.fil][st.col + 1];
			terreno[4] = matriz[st.fil - 2][st.col];
			terreno[5] = matriz[st.fil - 2][st.col + 1];
			terreno[6] = matriz[st.fil - 1][st.col + 2];
			terreno[7] = matriz[st.fil][st.col + 2];
			terreno[8] = matriz[st.fil - 3][st.col];
			terreno[9] = matriz[st.fil - 3][st.col + 1];
			terreno[10] = matriz[st.fil - 1][st.col + 3];
			terreno[11] = matriz[st.fil][st.col + 3];
			terreno[12] = matriz[st.fil - 2][st.col + 3];
			terreno[13] = matriz[st.fil - 1][st.col + 3];
			terreno[14] = matriz[st.fil][st.col + 3];
			terreno[15] = matriz[st.fil + 1][st.col + 3];
			break;

		case este:
			terreno[1] = matriz[st.fil - 1][st.col + 1];
			terreno[2] = matriz[st.fil][st.col + 1];
			terreno[3] = matriz[st.fil + 1][st.col + 1];
			terreno[4] = matriz[st.fil - 2][st.col + 2];
			terreno[5] = matriz[st.fil - 1][st.col + 2];
			terreno[6] = matriz[st.fil][st.col + 2];
			terreno[7] = matriz[st.fil + 1][st.col + 2];
			terreno[8] = matriz[st.fil + 2][st.col + 2];
			terreno[9] = matriz[st.fil - 3][st.col + 3];
			terreno[10] = matriz[st.fil - 2][st.col + 3];
			terreno[11] = matriz[st.fil - 1][st.col + 3];
			terreno[12] = matriz[st.fil][st.col + 3];
			terreno[13] = matriz[st.fil + 1][st.col + 3];
			terreno[14] = matriz[st.fil + 2][st.col + 3];
			terreno[15] = matriz[st.fil + 3][st.col + 3];
			break;

		case sureste:
			terreno[1] = matriz[st.fil][st.col + 1];
			terreno[2] = matriz[st.fil + 1][st.col + 1];
			terreno[3] = matriz[st.fil + 1][st.col];
			terreno[4] = matriz[st.fil][st.col + 2];
			terreno[5] = matriz[st.fil + 1][st.col + 2];
			terreno[6] = matriz[st.fil + 2][st.col + 2];
			terreno[7] = matriz[st.fil + 2][st.col + 1];
			terreno[8] = matriz[st.fil + 2][st.col];
			terreno[9] = matriz[st.fil][st.col + 3];
			terreno[10] = matriz[st.fil + 1][st.col + 3];
			terreno[11] = matriz[st.fil + 2][st.col + 3];
			terreno[12] = matriz[st.fil + 3][st.col + 3];
			terreno[13] = matriz[st.fil + 3][st.col + 2];
			terreno[14] = matriz[st.fil + 3][st.col + 1];
			terreno[15] = matriz[st.fil + 3][st.col];
			break;

		case sur:
			terreno[1] = matriz[st.fil + 1][st.col + 1];
			terreno[2] = matriz[st.fil + 1][st.col];
			terreno[3] = matriz[st.fil + 1][st.col - 1];
			terreno[4] = matriz[st.fil + 2][st.col + 2];
			terreno[5] = matriz[st.fil + 2][st.col + 1];
			terreno[6] = matriz[st.fil + 2][st.col];
			terreno[7] = matriz[st.fil + 2][st.col - 1];
			terreno[8] = matriz[st.fil + 2][st.col - 2];
			terreno[9] = matriz[st.fil + 3][st.col + 3];
			terreno[10] = matriz[st.fil + 3][st.col + 2];
			terreno[11] = matriz[st.fil + 3][st.col + 1];
			terreno[12] = matriz[st.fil + 3][st.col];
			terreno[13] = matriz[st.fil + 3][st.col - 1];
			terreno[14] = matriz[st.fil + 3][st.col - 2];
			terreno[15] = matriz[st.fil + 3][st.col - 3];
			break;

		case suroeste:
			terreno[1] = matriz[st.fil + 1][st.col];
			terreno[2] = matriz[st.fil + 1][st.col - 1];
			terreno[3] = matriz[st.fil][st.col - 1];
			terreno[4] = matriz[st.fil + 2][st.col];
			terreno[5] = matriz[st.fil + 2][st.col - 1];
			terreno[6] = matriz[st.fil + 2][st.col - 2];
			terreno[7] = matriz[st.fil + 1][st.col - 2];
			terreno[8] = matriz[st.fil][st.col - 2];
			terreno[9] = matriz[st.fil + 3][st.col];
			terreno[10] = matriz[st.fil + 3][st.col - 1];
			terreno[11] = matriz[st.fil + 3][st.col - 2];
			terreno[12] = matriz[st.fil + 3][st.col - 3];
			terreno[13] = matriz[st.fil + 2][st.col - 3];
			terreno[14] = matriz[st.fil + 1][st.col - 3];
			terreno[15] = matriz[st.fil][st.col - 3];
			break;

		case oeste:
			terreno[1] = matriz[st.fil + 1][st.col - 1];
			terreno[2] = matriz[st.fil][st.col - 1];
			terreno[3] = matriz[st.fil - 1][st.col - 1];
			terreno[4] = matriz[st.fil + 2][st.col - 2];
			terreno[5] = matriz[st.fil + 1][st.col - 2];
			terreno[6] = matriz[st.fil][st.col - 2];
			terreno[7] = matriz[st.fil - 1][st.col - 2];
			terreno[8] = matriz[st.fil - 2][st.col - 2];
			terreno[9] = matriz[st.fil + 3][st.col - 3];
			terreno[10] = matriz[st.fil + 2][st.col - 3];
			terreno[11] = matriz[st.fil + 1][st.col - 3];
			terreno[12] = matriz[st.fil][st.col - 3];
			terreno[13] = matriz[st.fil - 1][st.col - 3];
			terreno[14] = matriz[st.fil - 2][st.col - 3];
			terreno[15] = matriz[st.fil - 3][st.col - 3];
			break;

		case noroeste:
			terreno[1] = matriz[st.fil][st.col - 1];
			terreno[2] = matriz[st.fil - 1][st.col - 1];
			terreno[3] = matriz[st.fil - 1][st.col];
			terreno[4] = matriz[st.fil][st.col - 2];
			terreno[5] = matriz[st.fil - 1][st.col - 2];
			terreno[6] = matriz[st.fil - 2][st.col - 2];
			terreno[7] = matriz[st.fil - 2][st.col - 1];
			terreno[8] = matriz[st.fil - 2][st.col];
			terreno[9] = matriz[st.fil][st.col - 3];
			terreno[10] = matriz[st.fil - 1][st.col - 3];
			terreno[11] = matriz[st.fil - 2][st.col - 3];
			terreno[12] = matriz[st.fil - 3][st.col - 3];
			terreno[13] = matriz[st.fil - 3][st.col - 2];
			terreno[14] = matriz[st.fil - 3][st.col - 1];
			terreno[15] = matriz[st.fil - 3][st.col];
			break;
		}
	}
	else
	{
		switch (st.brujula)
		{
		case norte:
			terreno[1] = matriz[st.fil - 1][st.col - 1];
			terreno[2] = matriz[st.fil - 1][st.col];
			terreno[3] = matriz[st.fil - 1][st.col + 1];
			terreno[4] = matriz[st.fil - 2][st.col - 2];
			terreno[5] = matriz[st.fil - 2][st.col - 1];
			terreno[7] = matriz[st.fil - 2][st.col + 1];
			terreno[8] = matriz[st.fil - 2][st.col + 2];
			terreno[9] = matriz[st.fil - 3][st.col - 3];
			terreno[10] = matriz[st.fil - 3][st.col - 2];
			terreno[14] = matriz[st.fil - 3][st.col + 2];
			terreno[15] = matriz[st.fil - 3][st.col + 3];
			break;

		case noreste:
			terreno[1] = matriz[st.fil - 1][st.col];
			terreno[2] = matriz[st.fil - 1][st.col + 1];
			terreno[3] = matriz[st.fil][st.col + 1];
			terreno[4] = matriz[st.fil - 2][st.col];
			terreno[5] = matriz[st.fil - 2][st.col + 1];
			terreno[7] = matriz[st.fil - 1][st.col + 2];
			terreno[8] = matriz[st.fil][st.col + 2];
			terreno[9] = matriz[st.fil - 3][st.col];
			terreno[10] = matriz[st.fil - 3][st.col + 1];
			terreno[14] = matriz[st.fil - 1][st.col + 3];
			terreno[15] = matriz[st.fil][st.col + 3];
			break;

		case este:
			terreno[1] = matriz[st.fil - 1][st.col + 1];
			terreno[2] = matriz[st.fil][st.col + 1];
			terreno[3] = matriz[st.fil + 1][st.col + 1];
			terreno[4] = matriz[st.fil - 2][st.col + 2];
			terreno[5] = matriz[st.fil - 1][st.col + 2];
			terreno[7] = matriz[st.fil + 1][st.col + 2];
			terreno[8] = matriz[st.fil + 2][st.col + 2];
			terreno[9] = matriz[st.fil - 3][st.col + 3];
			terreno[10] = matriz[st.fil - 2][st.col + 3];
			terreno[14] = matriz[st.fil + 2][st.col + 3];
			terreno[15] = matriz[st.fil + 3][st.col + 3];
			break;

		case sureste:
			terreno[1] = matriz[st.fil][st.col + 1];
			terreno[2] = matriz[st.fil + 1][st.col + 1];
			terreno[3] = matriz[st.fil + 1][st.col];
			terreno[4] = matriz[st.fil][st.col + 2];
			terreno[5] = matriz[st.fil + 1][st.col + 2];
			terreno[7] = matriz[st.fil + 2][st.col + 1];
			terreno[8] = matriz[st.fil + 2][st.col];
			terreno[9] = matriz[st.fil][st.col + 3];
			terreno[10] = matriz[st.fil + 1][st.col + 3];
			terreno[14] = matriz[st.fil + 3][st.col + 1];
			terreno[15] = matriz[st.fil + 3][st.col];
			break;

		case sur:
			terreno[1] = matriz[st.fil + 1][st.col + 1];
			terreno[2] = matriz[st.fil + 1][st.col];
			terreno[3] = matriz[st.fil + 1][st.col - 1];
			terreno[4] = matriz[st.fil + 2][st.col + 2];
			terreno[5] = matriz[st.fil + 2][st.col + 1];
			terreno[7] = matriz[st.fil + 2][st.col - 1];
			terreno[8] = matriz[st.fil + 2][st.col - 2];
			terreno[9] = matriz[st.fil + 3][st.col + 3];
			terreno[10] = matriz[st.fil + 3][st.col + 2];
			terreno[14] = matriz[st.fil + 3][st.col - 2];
			terreno[15] = matriz[st.fil + 3][st.col - 3];
			break;

		case suroeste:
			terreno[1] = matriz[st.fil + 1][st.col];
			terreno[2] = matriz[st.fil + 1][st.col - 1];
			terreno[3] = matriz[st.fil][st.col - 1];
			terreno[4] = matriz[st.fil + 2][st.col];
			terreno[5] = matriz[st.fil + 2][st.col - 1];
			terreno[7] = matriz[st.fil + 1][st.col - 2];
			terreno[8] = matriz[st.fil][st.col - 2];
			terreno[9] = matriz[st.fil + 3][st.col];
			terreno[10] = matriz[st.fil + 3][st.col - 1];
			terreno[14] = matriz[st.fil + 1][st.col - 3];
			terreno[15] = matriz[st.fil][st.col - 3];
			break;

		case oeste:
			terreno[1] = matriz[st.fil + 1][st.col - 1];
			terreno[2] = matriz[st.fil][st.col - 1];
			terreno[3] = matriz[st.fil - 1][st.col - 1];
			terreno[4] = matriz[st.fil + 2][st.col - 2];
			terreno[5] = matriz[st.fil + 1][st.col - 2];
			terreno[7] = matriz[st.fil - 1][st.col - 2];
			terreno[8] = matriz[st.fil - 2][st.col - 2];
			terreno[9] = matriz[st.fil + 3][st.col - 3];
			terreno[10] = matriz[st.fil + 2][st.col - 3];
			terreno[14] = matriz[st.fil - 2][st.col - 3];
			terreno[15] = matriz[st.fil - 3][st.col - 3];
			break;

		case noroeste:
			terreno[1] = matriz[st.fil][st.col - 1];
			terreno[2] = matriz[st.fil - 1][st.col - 1];
			terreno[3] = matriz[st.fil - 1][st.col];
			terreno[4] = matriz[st.fil][st.col - 2];
			terreno[5] = matriz[st.fil - 1][st.col - 2];
			terreno[7] = matriz[st.fil - 2][st.col - 1];
			terreno[8] = matriz[st.fil - 2][st.col];
			terreno[9] = matriz[st.fil][st.col - 3];
			terreno[10] = matriz[st.fil - 1][st.col - 3];
			terreno[14] = matriz[st.fil - 3][st.col - 1];
			terreno[15] = matriz[st.fil - 3][st.col];
			break;
		}
	}
}
int ComportamientoJugador::BuscarCasillaMenosVista(vector<int> &terreno, int ini, int fin)
{
	int pos = 99;
	int min = 99;
	for (int i = ini; i <= fin; i++)
	{
		if (terreno[i] < min and terreno[i] > -1)
		{
			min = terreno[i];
			pos = i;
		}
	}
	return pos;
}