#ifndef CONTROLADOR_CONFIG_H
#define CONTROLADOR_CONFIG_H

#include <map>
#include <string>
#include <functional>

#include <def_video.h>
#include <herramientas/log_base/log_base.h>

#include <class/compositor_vista.h>
#include <templates/menu_opciones.h>

#include "../app/framework_impl/app_config.h"
#include "../app/fuentes.h"
#include "../app/localizador.h"
#include "../app/componente_menu.h"
#include "../app/fader.h"

#include "estados_controladores.h"
#include "../framework/controlador_interface.h"
#include "../app/framework_impl/input.h"

namespace App
{

class Controlador_config:
	public DFramework::Controlador_interface
{
	public:

					Controlador_config(DLibH::Log_base&, App_config&, const Fuentes& f, const Localizador& loc);

	virtual void 			preloop(DFramework::Input& input, float delta);
	virtual void 			loop(DFramework::Input& input, float delta);
	virtual void 			postloop(DFramework::Input& input, float delta);
	virtual void 			dibujar(DLibV::Pantalla& pantalla);
	virtual void 			despertar();
	virtual void 			dormir();
	virtual bool			es_posible_abandonar_estado() const;

	private:

	//Estructura para el componente de menú.
	struct item_menu
	{
		typedef std::function<void(DLibV::Representacion_agrupada&, int, int, const std::string&, const std::string&, const std::string&, bool)> func;

		func			f;
		std::string 		clave, nombre, valor;
		virtual void 		generar_representacion_listado(DLibV::Representacion_agrupada& r, int x, int y, bool actual) const {f(r, x, y, clave, nombre, valor, actual);}
		item_menu(func& f, const std::string& k, const std::string& n, const std::string& t):f(f), clave(k), nombre(n), valor(t) {}
	};

	void				crear_menu();
	void				inicializar_menu();
	void				salir();
	void				activar_fadeout();
	void				activar_menu(item_menu&);

	DLibH::Log_base&			log;
	App::App_config& 			config;
	const DLibV::Fuente_TTF&		fuente;
	const Localizador&			localizador;

	enum class 				modos{fadein, config, fadeout}modo;
	Componente_menu<item_menu, std::string>			componente_menu;
	Herramientas_proyecto::Compositor_vista			layout;
	Fader<float>						fader;
};

}

#endif
