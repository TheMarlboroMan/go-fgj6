#ifndef CONTROLADOR_INTRO_H
#define CONTROLADOR_INTRO_H

#include <def_video.h>
#include <class/compositor_vista.h>

#include "estados_controladores.h"

#include "../framework/controlador_interface.h"

#include "../app/localizador.h"
#include "../app/fuentes.h"
#include "../app/sistema_audio.h"


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

	enum class			modos{fadein, input, fadeout} modo;

	void				procesar_input(DFramework::Input& input);

	DLibH::Log_base&			log;
	const Localizador&			localizador;
	Sistema_audio&				sistema_audio;

	Herramientas_proyecto::Compositor_vista		layout;

	float				fade;
	bool				juego_finalizado;
};

}
#endif
