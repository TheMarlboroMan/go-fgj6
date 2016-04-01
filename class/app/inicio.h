#ifndef INICIO_H
#define INICIO_H

#include "espaciable.h"

namespace App
{

class Inicio
{
	public:

					Inicio(Espaciable::tpunto, int, int);

	Espaciable::tpunto		acc_punto() const {return punto;}
	int				acc_id() const {return id;}
	int 				acc_angulo() const {return angulo;}

	void				mut_id(int v) {id=v;}
	void				mut_angulo(int v) {angulo=v;}
	void				mut_punto(Espaciable::tpunto v) {punto=v;}

	private:

	Espaciable::tpunto		punto;
	int				id,
					angulo;
};

}

#endif
