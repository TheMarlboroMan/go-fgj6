#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "espaciable.h"

namespace App
{

class Obstaculo:
	public Espaciable
{
	public:

	enum class	ttipos{normal, inocuo};

					Obstaculo(const tpoligono&, ttipos);

	ttipos				acc_tipo() const {return tipo;}
	void				mut_tipo(ttipos v) {tipo=v;}

	private:

	ttipos				tipo;
};

}

#endif
