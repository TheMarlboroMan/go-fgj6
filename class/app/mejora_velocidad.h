#ifndef MEJORA_VELOCIDAD_H
#define MEJORA_VELOCIDAD_H

#include "espaciable.h"
#include "representable.h"

namespace App
{

class Mejora_velocidad:
	public Espaciable,
	public Representable
{
	public:

					Mejora_velocidad(Espaciable::tpunto, int);

	void				turno(float);
	bool				es_generar_particula() const {return !tiempo;}
	int				acc_nivel() const {return nivel;}
	void				mut_nivel(int v) {nivel=v;}

	virtual void 			dibujar(Representador&, DLibV::Pantalla&, const DLibV::Camara&) const;

	private:

	void				formar_poligono();

	int				nivel;
	float				tiempo;
};

}

#endif
