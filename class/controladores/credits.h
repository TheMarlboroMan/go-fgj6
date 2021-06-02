#pragma once

#include <def_video.h>
#include <class/compositor_vista.h>

#include "estados_controladores.h"

#include "../framework/controlador_interface.h"

#include "../app/localizador.h"
#include "../app/fuentes.h"
#include "../app/fader.h"
#include "../app/sistema_audio.h"

namespace App
{

class Controlador_credits:
	public DFramework::Controlador_interface
{
	public:
	
					Controlador_credits(DLibH::Log_base&, const Fuentes&, const Localizador&, Sistema_audio&);
	virtual void 			preloop(DFramework::Input& input, float delta);
	virtual void 			loop(DFramework::Input& input, float delta);
	virtual void 			postloop(DFramework::Input& input, float delta);
	virtual void 			dibujar(DLibV::Pantalla& pantalla);
	virtual void 			despertar();
	virtual void 			dormir();
	virtual bool			es_posible_abandonar_estado() const;

	private:

	enum class			modos{fadein, credits, fadeout} modo{modos::fadein};

	DLibH::Log_base&			log;
	const DLibV::Fuente_TTF&	fuente;
	const Localizador&			localizador;
	Sistema_audio&				sistema_audio;

	Herramientas_proyecto::Compositor_vista		layout;

	Fader<float>			fader;
};

}
