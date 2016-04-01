#ifndef PUERTA_H
#define PUERTA_H

#include "espaciable.h"

namespace App
{

class Puerta:
	public Espaciable
{
	public:

					Puerta(const tpoligono&, int);

	int				acc_id() const {return id;}
	void				mut_id(int v) {id=v;}

	private:

	int				id;
};

}

#endif
