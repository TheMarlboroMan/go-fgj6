#ifndef PARTICULA_H
#define PARTICULA_H

#include "espaciable.h"
#include "representable.h"


namespace App
{

class Particula:
	public Representable
{
	public:

					Particula(Espaciable::tpunto, double, float);
	virtual void			turno(float)=0;
	virtual bool			es_borrar()const=0;
	virtual void 			dibujar(Representador&, DLibV::Pantalla&, const DLibV::Camara&) const=0;

	protected:

	Espaciable::tpunto		centro;
	double 				angulo;
	float				velocidad;
};

}

#endif
