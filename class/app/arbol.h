#ifndef ARBOL_H
#define ARBOL_H

#include <map>

#include "espaciable.h"
#include "representable.h"

namespace App
{

class Arbol:
	public Espaciable,
	public Representable
{
	public:

					Arbol(Espaciable::tpunto);

	void				turno(float);
	void				colocar_pieza(int);
	bool				es_finalizado() const;
	bool				es_florecido() const {return tiempo_florecimiento==5.f;}
	virtual void 			dibujar(Representador&, DLibV::Pantalla&, const DLibV::Camara&) const;

	private:

	void				formar_poligono();

	std::map<int, bool>		piezas_colocadas;
	
	float				tiempo_florecimiento;

	
};

}

#endif
