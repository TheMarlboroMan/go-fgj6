#ifndef CONTROLADOR_INTRO_H
#define CONTROLADOR_INTRO_H

#include <def_video.h>
#include <class/compositor_vista.h>

#include "estados_controladores.h"

#include "../framework/controlador_interface.h"

#include "../app/localizador.h"
#include "../app/fuentes.h"
#include "../app/fader.h"
#include "../app/sistema_audio.h"
#include "../app/componente_menu.h"

namespace App
{

class Controlador_intro:
	public DFramework::Controlador_interface
{
	public:
	
					Controlador_intro(DLibH::Log_base&, const Fuentes&, const Localizador&, Sistema_audio&);
	virtual void 			preloop(DFramework::Input& input, float delta);
	virtual void 			loop(DFramework::Input& input, float delta);
	virtual void 			postloop(DFramework::Input& input, float delta);
	virtual void 			dibujar(DLibV::Pantalla& pantalla);
	virtual void 			despertar();
	virtual void 			dormir();
	virtual bool			es_posible_abandonar_estado() const;

	void				establecer_finalizado() {juego_finalizado=true;}

	private:

	//Estructura para el componente de menú.
	struct item_menu
	{
		typedef std::function<void(DLibV::Representacion_agrupada&, int, int, const std::string&, bool)> func;

		func			f;
		std::string 		clave, nombre;
		virtual void 		generar_representacion_listado(DLibV::Representacion_agrupada& r, int x, int y, bool actual) const {f(r, x, y, nombre, actual);}
		item_menu(func& f, const std::string& k, const std::string& n):f(f), clave(k), nombre(n) {}
	};

	enum class			modos{fadein, esperando, transicion, menu, fadeout, fade_finalizado} modo;

	void				inicializar_menu();
	void				procesar_input(DFramework::Input& input);
	void				procesar_fade(float);

	DLibH::Log_base&			log;
	const DLibV::Fuente_TTF&		fuente;
	const Localizador&			localizador;
	Sistema_audio&				sistema_audio;

	Herramientas_proyecto::Compositor_vista		layout;

	Fader<float>			fade, fade_out, fade_menu;
	bool				juego_finalizado;

	Componente_menu<item_menu, std::string>		componente_menu;
};

}
#endif
