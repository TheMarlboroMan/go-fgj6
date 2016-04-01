#ifndef INTERRUPTOR_H
#define INTERRUPTOR_H

#include "espaciable.h"

namespace App
{

class Interruptor:
	public Espaciable
{
	public:

					Interruptor(Espaciable::tpunto, int, int, int, int);

	int				acc_nivel() const {return nivel;}
	int				acc_id_puerta() const {return id_puerta;}
	int				acc_id_grupo() const {return id_grupo;}
	int				acc_tiempo_grupo() const {return tiempo_grupo;}

	void				mut_nivel(int v) {nivel=v;}
	void				mut_id_puerta(int v) {id_puerta=v;}
	void				mut_id_grupo(int v) {id_grupo=v;}
	void				mut_tiempo_grupo(int v) {tiempo_grupo=v;}

	private:

	void				formar_poligono();

	int				nivel,
					id_puerta,
					id_grupo,
					tiempo_grupo;
};

}

#endif
