#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
using namespace std;
struct state{
  int fil;
  int col;
  Orientacion brujula;
};

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      current_state.fil = 99;
      current_state.col = 99;
      current_state.brujula = norte;
      girar_derecha = false;
      bien_situado = false;
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void PonerTerrenoMatriz(const vector<unsigned char> &terreno, const state &st, vector <vector< unsigned char> > &matriz);
  private:
  // Declarar aquí las variables de estado
  state current_state;
  Orientacion brujula;
  Action last_action;
  bool girar_derecha;
  bool bien_situado;
};
#endif
