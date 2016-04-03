#ifndef COLA_VIENTO_H
#define COLA_VIENTO_H

#include "espaciable.h"
#include "representable.h"


namespace App
{

class Cola_viento:
	public Representable
{
	public:

					Cola_viento(Espaciable::tpunto, double, float);
	void				turno(float);
	bool				es_borrar() const {return tiempo >= 0.5f;}
	virtual void 			dibujar(Representador&, DLibV::Pantalla&, const DLibV::Camara&) const;

	private:

	void				movimiento(float);

	Espaciable::tpunto		centro;
	double 				angulo;
	float				velocidad,
					tiempo;
	int				rep;
};

}

#endif
