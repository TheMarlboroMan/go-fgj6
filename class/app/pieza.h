#ifndef PIEZA_H
#define PIEZA_H

#include "espaciable.h"
#include "representable.h"

namespace App
{

class Pieza:
	public Espaciable,
	public Representable
{
	public:

					Pieza(Espaciable::tpunto, int);

	int				acc_indice() const {return indice;}
	void				mut_indice(int v) {indice=v;}
	virtual void 			dibujar(Representador&, DLibV::Pantalla&, const DLibV::Camara&) const;

	private:

	void				formar_poligono();

	int				indice;
};

}

#endif
