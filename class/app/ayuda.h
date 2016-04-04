#ifndef AYUDA_H
#define AYUDA_H

#include <class/valor_limitado.h>

#include "espaciable.h"
#include "representable.h"

namespace App
{

class Ayuda:
	public Espaciable,
	public Representable
{
	public:

					Ayuda(Espaciable::tpunto, int);

	int				acc_indice() const {return indice;}
	void				mut_indice(int v) {indice=v;}
	void				turno(float);
	void				activar() {tiempo_activo=15.0f;}
	bool				es_activable() {return !tiempo_activo;}
	virtual void 			dibujar(Representador&, DLibV::Pantalla&, const DLibV::Camara&) const;

	private:

	void				formar_poligono();
	
	int				indice;
	Herramientas_proyecto::Valor_limitado<float>	tiempo_activo;
};

}

#endif
