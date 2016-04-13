#ifndef PIEZA_ANIMACION_H
#define PIEZA_ANIMACION_H

#include "espaciable.h"
#include "representable.h"

namespace App
{

class Pieza_animacion:
	public Espaciable,
	public Representable
{
	public:

				Pieza_animacion();
	void 			activar(int, Espaciable::tpunto, Espaciable::tpunto);
	void			turno(float);
	void			reiniciar();
	bool			es_activa() const {return estado!=estados::inactiva;}
	bool			es_generar_hoja() const {return tiempo==0.0f;}
	int			acc_pieza_actual() const {return id_pieza_actual;}

	virtual void		dibujar(Representador&, DLibV::Pantalla&, const DLibV::Camara&) const;

	private:

	void			formar_poligono();

	enum class 		estados{inactiva, movimiento, crecimiento}estado;
	int			id_pieza_actual;
	Espaciable::tpunto	destino;
	double			angulo;
	float			factor_tam,
				alpha,
				tiempo;
};

}

#endif
