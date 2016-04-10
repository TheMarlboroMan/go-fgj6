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
	virtual void 			dibujar(Representador&, DLibV::Pantalla&, const DLibV::Camara&) const;

	void				cancelar_movimiento(float delta);
	int				acc_indice_velocidad() const {return indice_velocidad;}
	int				acc_max_velocidad() const {return max_velocidad;}
	int				acc_tope_velocidad() const {return velocidades.size()-1;}
	int				acc_pieza_actual() const {return pieza_actual;}
	void				mut_pieza_actual(int v) {pieza_actual=v;}
	void				establecer_max_velocidad(int v) {if(v > max_velocidad) max_velocidad=v;}
	void				reiniciar();
	bool				es_generar_cola() {return !t_cola;}
	bool				es_sonido_velocidad() const {return sonido_velocidad;}

	float				acc_angulo() const {return angulo;}
	float				acc_velocidad() const {return velocidad;}

	private:

	static const float 		factor_rotacion;
	static const float 		factor_min_rotacion;
	static const float 		factor_aceleracion;

	void				formar_poligono();
	void				cambiar_velocidad(int);
	void				girar(int, float);
	void				movimiento(float);

	Bloque_input			input_actual;
	double 				angulo;
	float				velocidad, velocidad_destino;
	int 				indice_velocidad,
					pieza_actual,
					max_velocidad;
	float				tiempo, t_cola;
	std::vector<float>		velocidades;
	bool				sonido_velocidad;
	
};

}

#endif
