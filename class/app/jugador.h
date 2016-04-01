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
	int 				indice_velocidad;
	std::vector<float>		velocidades;
	
};

}

#endif
