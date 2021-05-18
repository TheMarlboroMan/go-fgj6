#ifndef COLA_VIENTO_H
#define COLA_VIENTO_H

#include <templates/valor_limitado.h>

#include "particula.h"

namespace App
{

class Cola_viento:
	public Particula
{
	public:

					Cola_viento(Espaciable::tpunto, double, float, float t);
	virtual void			turno(float);
	virtual bool			es_borrar() const {return tiempo <= 0.f;}
	virtual void 			dibujar(Representador&, DLibV::Pantalla&, const DLibV::Camara&) const;

	private:

	void				movimiento(float);

	Herramientas_proyecto::Valor_limitado<float>	tiempo;
	int				rep;
};

}

#endif
