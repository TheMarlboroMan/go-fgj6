#ifndef DECORACION_H
#define DECORACION_H

#include "color.h"
#include "espaciable.h"
#include "representable.h"

namespace App
{

class Decoracion:
	public Espaciable,
	public Representable
{
	public:

					Decoracion(const tpoligono&, tcolor, tcolor, bool, int);
	tcolor				acc_color() const {return color;}
	tcolor				acc_color_linea() const {return color_linea;}
	bool				es_frente() const {return frente;}
	int				acc_profundidad() const {return profundidad;}

	void				mut_color(tcolor c) {color=c;}
	void				mut_color_linea(tcolor c) {color_linea=c;}
	void				mut_frente(bool v) {frente=v;}
	void				mut_profundidad(int v);
	void				subir_profundidad();
	void				bajar_profundidad();

	bool				operator<(const Decoracion&) const;

	virtual void 			dibujar(Representador&, DLibV::Pantalla&, const DLibV::Camara&) const;

	private:

	void				comprobar_validez_profundidad();

	static const int		min_profundidad=0;
	static const int		max_profundidad=200;

	tcolor				color,
					color_linea;
	bool				frente;
	int				profundidad;
};

}

#endif
