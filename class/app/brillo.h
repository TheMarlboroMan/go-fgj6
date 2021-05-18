#ifndef BRILLO_H
#define BRILLO_H

#include <templates/valor_limitado.h>

#include "particula.h"

namespace App
{

class Brillo:
	public Particula
{
	public:

					Brillo(Espaciable::tpunto, double, float, float t);
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
