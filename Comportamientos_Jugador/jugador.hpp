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
    fil = 99;
    col = 99;
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
    pos = -1;
    ini_norte = norte;
    ini_sur = sur;
    ini_este = este;
    ini_oeste = oeste;
    a_norte = ini_norte;
    a_este = ini_este;
    a_sur = ini_sur;
    a_oeste = ini_oeste;

    // Matriz auxiliar para guardar la cantidad de veces que hemos pisado/visto cada casilla
    filas = mapaResultado.size();
    columnas = mapaResultado[0].size();
    mapaPasos = vector<vector<int>>(200, vector<int>(200));
    // Declaración de la matriz con tamaño 200x200
    mapaNoSituado = vector<vector<unsigned char>>(200, vector<unsigned char>(200, 0));
  }

  ComportamientoJugador(const ComportamientoJugador &comport) : Comportamiento(comport) {}
  ~ComportamientoJugador() {}

  Action think(Sensores sensores);
  int interact(Action accion, int valor);
  void PonerTerrenoMatriz(const vector<unsigned char> &terreno, const state &st, vector<vector<unsigned char>> &matriz, Orientacion pos, Sensores sensor);
  void PonerTerrenoMatrizNoSituado(const vector<unsigned char> &terreno, const state &st, vector<vector<unsigned char>> &matriz, Orientacion pos, Sensores sensor);
  int HayCasilaEspecialNecesariaEnVista(const vector<unsigned char> &terreno, Sensores sensor);
  Action IrCasillaEnVista(int pos, const vector<unsigned char> &terreno);
  void PintarPrecicpicios(bool pintar);
  void PonerCantidadEnAuxiliar(const state &current_state, vector<vector<int>> &matriz, Orientacion pos, Sensores sensor);

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
  int fil;
  int col;
  int a;
  int pos;
  int a_norte;
  int a_este;
  int a_sur;
  int a_oeste;
  Orientacion ini_norte;
  Orientacion ini_sur;
  Orientacion ini_este;
  Orientacion ini_oeste;
  vector<vector<int>> mapaPasos;
  vector<vector<unsigned char>> mapaNoSituado;
};
#endif
