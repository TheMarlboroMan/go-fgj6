#ifndef CONTROLADOR_CONTROLES_H
#define CONTROLADOR_CONTROLES_H

#include <map>
#include <string>

#include <def_video.h>
#include <class/compositor_vista.h>
#include <herramientas/log_base/log_base.h>

#include "../app/framework_impl/app_config.h"
#include "../app/fuentes.h"
#include "../app/localizador.h"

#include "estados_controladores.h"
#include "../framework/controlador_interface.h"

namespace App
{

class Controlador_controles:
	public DFramework::Controlador_interface
{
	public:

					Controlador_controles(DLibH::Log_base&, App_config&, const Fuentes& f, const Localizador& loc, const DFramework::Input& i);

	virtual void 			preloop(DFramework::Input& input, float delta);
	virtual void 			loop(DFramework::Input& input, float delta);
	virtual void 			postloop(DFramework::Input& input, float delta);
	virtual void 			dibujar(DLibV::Pantalla& pantalla);
	virtual void 			despertar();
	virtual void 			dormir();
	virtual bool			es_posible_abandonar_estado() const;

	private:

	void				preparar_controles(const DFramework::Input& input);
	void				finalizar_configuracion();
	std::string			traducir_input(const DFramework::Input::Entrada& e) const;

/*
	struct tinput{
		enum tipos {arriba, abajo, izquierda, derecha};
					
		int 				jugador, tipo, clave;
		std::string 			nombre;
		DFramework::Input::Entrada 	entrada;
	};
*/

	DLibH::Log_base&			log;
	App::App_config& 			config;
	const DLibV::Fuente_TTF&		fuente;
	const Localizador&			localizador;

	Herramientas_proyecto::Compositor_vista		layout;
//	std::map<int, tinput>			controles;
//	std::string				str_controles, str_actual;
//	int					kactual;
};

}

#endif
