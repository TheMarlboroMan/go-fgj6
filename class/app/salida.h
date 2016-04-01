#ifndef SALIDA_H
#define SALIDA_H

#include "espaciable.h"

namespace App
{

class Salida:
	public Espaciable
{
	public:

					Salida(const tpoligono&, int, int);

	int				acc_id_mapa() const {return id_mapa;}
	int				acc_id_inicio() const {return id_inicio;}

	void				mut_id_mapa(int v) {id_mapa=v;}
	void				mut_id_inicio(int v) {id_inicio=v;}

	private:

	int				id_mapa,
					id_inicio;
};

}

#endif
