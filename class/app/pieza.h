#ifndef PIEZA_H
#define PIEZA_H

#include "espaciable.h"

namespace App
{

class Pieza:
	public Espaciable
{
	public:

					Pieza(Espaciable::tpunto, int);

	int				acc_indice() const {return indice;}
	void				mut_indice(int v) {indice=v;}

	private:

	void				formar_poligono();

	int				indice;
};

}

#endif
