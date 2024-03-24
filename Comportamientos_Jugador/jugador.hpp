#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
using namespace std;
struct state
{
  int fil;
  int col;
  Orientacion brujula;
};

class ComportamientoJugador : public Comportamiento
{

public:
  ComportamientoJugador(unsigned int size) : Comportamiento(size)
  {
    current_state.fil = 99;
    current_state.col = 99;
    current_state.brujula = norte;
    girar_derecha = false;
    bien_situado = false;
    primera_vez = true;
    hay_casilla_posicion = false;
    hay_casilla_recuperacion = false;
    hay_casilla_zapatillas = false;
    hay_casilla_bikini = false;
    tengo_zapatilllas = false;
    tengo_bikini = false;
    hay_algo = false;
    busco_bateria = false;
    imposible = false;

    // Matriz auxiliar para guardar la cantidad de veces que hemos pisado/visto cada casilla
    filas = mapaResultado.size();
    columnas = mapaResultado[0].size();
    mapaPasos = vector<vector<unsigned char>>(filas, vector<unsigned char>(columnas));
  }

  ComportamientoJugador(const ComportamientoJugador &comport) : Comportamiento(comport) {}
  ~ComportamientoJugador() {}

  Action think(Sensores sensores);
  int interact(Action accion, int valor);
  void PonerTerrenoMatriz(const vector<unsigned char> &terreno, const state &current_state, vector<vector<unsigned char>> &matriz, Orientacion pos, Sensores sensor);
  int HayCasilaEspecialNecesariaEnVista(const vector<unsigned char> &terreno, Sensores sensor);
  Action IrCasillaEnVista(int pos, const vector<unsigned char> &terreno);
  void PintarPrecicpicios(bool pintar);
  void PonerCantidadEnAuxiliar(const state &current_state, vector<vector<unsigned char>> &matriz, Orientacion pos, Sensores sensor);

private:
  // Declarar aquí las variables de estado
  state current_state;
  Orientacion brujula;
  Action last_action;
  bool girar_derecha;
  bool bien_situado;
  bool primera_vez;
  bool hay_casilla_posicion;
  bool hay_casilla_recuperacion;
  bool hay_casilla_zapatillas;
  bool hay_casilla_bikini;
  bool tengo_zapatilllas;
  bool tengo_bikini;
  bool hay_algo;
  bool busco_bateria;
  bool imposible;
  int filas;
  int columnas;
  vector<vector<int>> mapaPasos;
  vector<vector<unsigned char>> mapaNoSituado;
};
#endif
