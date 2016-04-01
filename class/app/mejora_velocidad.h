#ifndef MEJORA_VELOCIDAD_H
#define MEJORA_VELOCIDAD_H

#include "espaciable.h"

namespace App
{

class Mejora_velocidad:
	public Espaciable
{
	public:

					Mejora_velocidad(Espaciable::tpunto, int);

	int				acc_nivel() const {return nivel;}
	void				mut_nivel(int v) {nivel=v;}

	private:

	void				formar_poligono();

	int				nivel;
};

}

#endif
