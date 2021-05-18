#ifndef INTERRUPTOR_H
#define INTERRUPTOR_H

#include <templates/valor_limitado.h>

#include "espaciable.h"
#include "representable.h"

namespace App
{

class Interruptor:
	public Espaciable,
	public Representable
{
	public:

					Interruptor(Espaciable::tpunto, int, int, int, int);

	int				acc_nivel() const {return nivel;}
	int				acc_id_puerta() const {return id_puerta;}
	int				acc_id_grupo() const {return id_grupo;}
	int				acc_tiempo_grupo() const {return tiempo_grupo;}
	bool				es_activo() const {return tiempo_activo > 0.f;}
	void				turno(float delta);
	void				activar();

	void				mut_nivel(int v) {nivel=v;}
	void				mut_id_puerta(int v) {id_puerta=v;}
	void				mut_id_grupo(int v) {id_grupo=v;}
	void				mut_tiempo_grupo(int v) {tiempo_grupo=v;}

	virtual void 			dibujar(Representador&, DLibV::Pantalla&, const DLibV::Camara&) const;

	private:

	void				formar_poligono();

	int				nivel,
					id_puerta,
					id_grupo,
					tiempo_grupo;

	float				angulo;

	Herramientas_proyecto::Valor_limitado<float>	tiempo_activo;
};

}

#endif
