#ifndef PUERTA_H
#define PUERTA_H

#include "espaciable.h"
#include "representable.h"

namespace App
{

class Puerta:
	public Espaciable,
	public Representable
{
	public:

					Puerta(const tpoligono&, int);

	int				acc_id() const {return id;}
	void				mut_id(int v) {id=v;}

	virtual void 			dibujar(Representador&, DLibV::Pantalla&, const DLibV::Camara&) const;

	private:

	int				id;
};

}

#endif
