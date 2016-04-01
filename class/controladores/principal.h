#ifndef CONTROLADOR_PRINCIPAL_H
#define CONTROLADOR_PRINCIPAL_H

#include <vector>
#include <memory>

#include <def_video.h>

#include <herramientas/log_base/log_base.h>

#include "../app/fuentes.h"
#include "../app/mapa.h"
#include "../app/jugador.h"
#include "../app/bloque_input.h"

#include "estados_controladores.h"
#include "../framework/controlador_interface.h"

namespace App
{

class Controlador_principal:
	public DFramework::Controlador_interface
{
	public:

					Controlador_principal(DLibH::Log_base&, const Fuentes& f);

	virtual void 			preloop(DFramework::Input& input, float delta);
	virtual void 			loop(DFramework::Input& input, float delta);
	virtual void 			postloop(DFramework::Input& input, float delta);
	virtual void 			dibujar(DLibV::Pantalla& pantalla);
	virtual void 			despertar();
	virtual void 			dormir();
	virtual bool			es_posible_abandonar_estado() const;

	Mapa&				acc_mapa() {return mapa;}

	private:

	void					ajustar_camara();
	void					procesar_jugador(DFramework::Input&, float, Jugador&);
	Bloque_input				obtener_bloque_input(DFramework::Input& input) const;
	void					jugador_en_salida(const Salida&); 

	DLibH::Log_base&			log;
	const DLibV::Fuente_TTF&		fuente_akashi;

	DLibV::Camara				camara;
	Mapa					mapa;
	Jugador					jugador;

	struct
	{
		Inicio				inicio_actual;
	}info_mapa{Inicio(Espaciable::tpunto{0.0,0.0}, 0, 0)};
};

}

#endif
