#include "intro.h"

#include "../app/framework_impl/input.h"

using namespace App;

Controlador_intro::Controlador_intro(DLibH::Log_base& log, const Fuentes& fuentes, const Localizador& loc)
	:log(log), localizador(loc), fade(255.0f)
{
	layout.mapear_fuente("fuente", fuentes.obtener_fuente("imagination_station", 16));
	layout.mapear_textura("cover", DLibV::Gestor_texturas::obtener(7));
}

void Controlador_intro::preloop(DFramework::Input& input, float delta)
{

}

void Controlador_intro::postloop(DFramework::Input& input, float delta)
{

}

void Controlador_intro::loop(DFramework::Input& input, float delta)
{
	if(input.es_senal_salida() || input.es_input_down(Input::escape))
	{
		abandonar_aplicacion();
	}
	else
	{
		if(fade > 1.0f)
		{
			fade-=delta*50.0f;
			if(fade < 1.0f) fade=1.0f;
			int ffade=floor(fade);
			layout.obtener_por_id("fader")->establecer_alpha(ffade);

			int y=ffade < 130 ? 130 : ffade;
			layout.obtener_por_id("cover")->ir_a(140, y);
		}

		if(input.hay_eventos_teclado_down() && fade <= 200.0)
		{
			solicitar_cambio_estado(principal);
		}
	}
}

void Controlador_intro::dibujar(DLibV::Pantalla& pantalla)
{
	layout.volcar(pantalla);
}

void Controlador_intro::despertar()
{
	layout.parsear("data/layout/layout_intro.dnot", "layout");
	fade=255.0f;
}

void Controlador_intro::dormir()
{
	layout.vaciar_vista();
}

bool Controlador_intro::es_posible_abandonar_estado() const
{
	return true;
}
