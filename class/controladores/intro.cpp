#include "intro.h"

#include "../app/framework_impl/input.h"

using namespace App;

Controlador_intro::Controlador_intro(DLibH::Log_base& log, const Fuentes& fuentes, const Localizador& loc, Sistema_audio& sa)
	:log(log), localizador(loc), sistema_audio(sa), fade(255.0f), juego_finalizado(false)
{
	layout.mapear_fuente("fuente", fuentes.obtener_fuente("imagination_station", 16));
	layout.mapear_textura("cover", DLibV::Gestor_texturas::obtener(7));
	layout.mapear_textura("flores", DLibV::Gestor_texturas::obtener(8));
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
		switch(modo)
		{
			case modos::fadein:
				if(fade > 1.0f)
				{
					fade-=delta*50.0f;
					if(fade < 1.0f) fade=1.0f;
					int ffade=floor(fade);
					layout.obtener_por_id("fader")->establecer_alpha(ffade);

					int y=ffade < 130 ? 130 : ffade;
					layout.obtener_por_id("cover")->ir_a(140, y);
					layout.obtener_por_id("flores")->ir_a(140, y);
				}

				procesar_input(input);
			break;
			case modos::input:
				procesar_input(input);
			break;
			case modos::fadeout:

				if(fade < 254.0f)
				{
					fade+=delta*150.0f;
					if(fade > 254.0f) fade=254.f;

					int ffade=ceil(fade);
					layout.obtener_por_id("fader")->establecer_alpha(ffade);
					if(juego_finalizado) layout.obtener_por_id("flores")->establecer_alpha(ffade);

					if(fade==254.f)
					{
						solicitar_cambio_estado(principal);
						modo=modos::fadein;
					}
				}
			break;
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

	auto r=static_cast<DLibV::Representacion_bitmap *>(layout.obtener_por_id("flores"));
	r->establecer_modo_blend(DLibV::Representacion::BLEND_ALPHA);
	if(juego_finalizado) layout.obtener_por_id("flores")->establecer_alpha(255);

	fade=255.0f;
	modo=modos::fadein;
}

void Controlador_intro::dormir()
{
	layout.vaciar_vista();
}

bool Controlador_intro::es_posible_abandonar_estado() const
{
	return true;
}

void Controlador_intro::procesar_input(DFramework::Input& input)
{
	if(input.hay_eventos_teclado_down() && fade <= 200.0)
	{
		sistema_audio.insertar(Info_audio_reproducir(
			Info_audio_reproducir::t_reproduccion::simple,
			Info_audio_reproducir::t_sonido::repetible,
			5, 127, 127));

		modo=modos::fadeout;
	}
}