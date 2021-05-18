#ifndef CONTROLADOR_CONTROLES_H
#define CONTROLADOR_CONTROLES_H

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

class Controlador_controles:
	public DFramework::Controlador_interface
{
	public:

					Controlador_controles(DLibH::Log_base&, App_config&, const Fuentes& f, const Localizador& loc, const DFramework::Input&);

	virtual void 			preloop(DFramework::Input& input, float delta);
	virtual void 			loop(DFramework::Input& input, float delta);
	virtual void 			postloop(DFramework::Input& input, float delta);
	virtual void 			dibujar(DLibV::Pantalla& pantalla);
	virtual void 			despertar();
	virtual void 			dormir();
	virtual bool			es_posible_abandonar_estado() const;

	private:

	//Estructura para el componente de menú.
	struct item_config_controles
	{
		typedef std::function<void(DLibV::Representacion_agrupada&, int, int, const std::string&, const std::string&, const std::string&, bool)> func;

		func			f;
		std::string 		clave, nombre, valor;
		virtual void 		generar_representacion_listado(DLibV::Representacion_agrupada& r, int x, int y, bool actual) const {f(r, x, y, clave, nombre, valor, actual);}
		item_config_controles(func& f, const std::string& k, const std::string& n, const std::string& t):f(f), clave(k), nombre(n), valor(t) {}
	};


	void				crear_menu_opciones(const DFramework::Input& input);
	void				actualizar_configuracion(const std::string&, App_config::input_jugador);
	void				generar_vista_menu();
	std::string			traducir_input(const App_config::input_jugador& e) const;
	int				indice_input_desde_clave(const std::string&) const;
	int				indice_traduccion_desde_clave(const std::string&) const;
	App_config::input_jugador	input_desde_string(const std::string&) const;
	std::string			string_desde_input(const App_config::input_jugador&) const;
	void				salir();
	void				activar_fadeout();
	void				input_seleccion(DFramework::Input& input);
	void				input_entrada(DFramework::Input& input);

	DLibH::Log_base&			log;
	App::App_config& 			config;
	const DLibV::Fuente_TTF&		fuente;
	const Localizador&			localizador;

	enum class 				modos{fadein, seleccion, entrada, fadeout}modo;
	Componente_menu<item_config_controles, std::string>	componente_menu;
	Herramientas_proyecto::Compositor_vista			layout;
	Fader<float>						fader;
};

}

#endif
