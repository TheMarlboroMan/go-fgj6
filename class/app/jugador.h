#ifndef JUGADOR_H
#define JUGADOR_h

#include <map>

#include "espaciable.h"
#include "representable.h"
#include "bloque_input.h"

namespace App
{

class Jugador:
	public Espaciable,
	public Representable
{
	public:

					Jugador();
	void				recibir_input(const Bloque_input&);
	void				establecer_inicio(Espaciable::tpunto, int);
	void				turno(float);
	void				colisionar(bool);
	virtual void 			dibujar(Representador&, DLibV::Pantalla&, const DLibV::Camara&) const;

	int				acc_indice_velocidad() const {return indice_velocidad;}
	int				acc_pieza_actual() const {return pieza_actual;}
	void				mut_pieza_actual(int v) {pieza_actual=v;}
	void				establecer_max_velocidad(int v) {if(v > max_velocidad) max_velocidad=v;}

	private:

	static const float 		factor_rotacion;
	static const float 		factor_min_rotacion;
	static const float 		factor_aceleracion;

	void				formar_poligono();
	void				cambiar_velocidad(int);
	void				detener_velocidad(float);
	void				girar(int, float);
	void				movimiento(float);

	Bloque_input			input_actual;
	double 				angulo;
	float				velocidad;
	int 				indice_velocidad,
					pieza_actual,
					max_velocidad;
	std::vector<float>		velocidades;
	
};

}

#endif
