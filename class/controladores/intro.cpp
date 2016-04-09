#include "intro.h"

#include "../app/framework_impl/input.h"
#include "../app/localizacion.h"

using namespace App;

Controlador_intro::Controlador_intro(DLibH::Log_base& log, const Fuentes& fuentes, const Localizador& loc, Sistema_audio& sa)
	:log(log), localizador(loc), sistema_audio(sa), fade(255.0f), fade_menu(0.0f), juego_finalizado(false),
	indice_menu(0)
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
		procesar_fade(delta);
		procesar_input(input);
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

	static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id("txt_pulsa_tecla"))->asignar(localizador.obtener(Localizacion::intro_pulsa_algo));
	static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id("txt_inicio_on"))->asignar(localizador.obtener(Localizacion::intro_empezar));
	static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id("txt_inicio_off"))->asignar(localizador.obtener(Localizacion::intro_empezar));
	static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id("txt_controles_on"))->asignar(localizador.obtener(Localizacion::intro_controles));
	static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id("txt_controles_off"))->asignar(localizador.obtener(Localizacion::intro_controles));
	static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id("txt_salir_on"))->asignar(localizador.obtener(Localizacion::intro_salir));
	static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id("txt_salir_off"))->asignar(localizador.obtener(Localizacion::intro_salir));

	fade=255.0f;
	fade_menu=0.0f;
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
	switch(modo)
	{
		case modos::fadein:
		case modos::esperando:
			if(input.hay_eventos_teclado_down() && fade <= 200.0)
			{
				modo=modos::transicion;
				refrescar_menu();
			}
		break;
		case modos::menu:
		{
			if(input.es_input_down(Input::menu_arriba) && indice_menu > min_menu)
			{
				--indice_menu;
				refrescar_menu();
			}
			else if(input.es_input_down(Input::menu_abajo) && indice_menu < max_menu)
			{
				++indice_menu;
				refrescar_menu();
			}
			else if(input.es_input_down(Input::menu_ok))
			{
				sistema_audio.insertar(Info_audio_reproducir(
					Info_audio_reproducir::t_reproduccion::simple,
					Info_audio_reproducir::t_sonido::repetible,
					5, 127, 127));

				modo=modos::fadeout;
			}
		}
		break;

		case modos::transicion:
		case modos::fadeout:
		break;
	}
}

void Controlador_intro::refrescar_menu()
{
	layout.obtener_por_id("txt_inicio_on")->hacer_invisible();
	layout.obtener_por_id("txt_controles_on")->hacer_invisible();
	layout.obtener_por_id("txt_salir_on")->hacer_invisible();

	layout.obtener_por_id("txt_inicio_off")->hacer_visible();
	layout.obtener_por_id("txt_controles_off")->hacer_visible();
	layout.obtener_por_id("txt_salir_off")->hacer_visible();

	switch(indice_menu)
	{
		case 0: layout.obtener_por_id("txt_inicio_on")->hacer_visible(); break;
		case 1: layout.obtener_por_id("txt_controles_on")->hacer_visible(); break;
		case 2: layout.obtener_por_id("txt_salir_on")->hacer_visible(); break;
	}
}

void Controlador_intro::procesar_fade(float delta)
{
	if(modo==modos::transicion)
	{
		if(fade_menu < 254.0f)
		{
			fade_menu+=delta*150.0f;
			if(fade_menu > 254.0f) fade_menu=254.f;

			int ffade=ceil(fade_menu);

			layout.obtener_por_id("txt_pulsa_tecla")->establecer_alpha(255 - ffade);
			layout.obtener_por_id("txt_inicio_on")->establecer_alpha(ffade);
			layout.obtener_por_id("txt_inicio_off")->establecer_alpha(ffade);
			layout.obtener_por_id("txt_controles_on")->establecer_alpha(ffade);
			layout.obtener_por_id("txt_controles_off")->establecer_alpha(ffade);
			layout.obtener_por_id("txt_salir_on")->establecer_alpha(ffade);
			layout.obtener_por_id("txt_salir_off")->establecer_alpha(ffade);

			if(fade_menu==254.f)
			{
				modo=modos::menu;
			}
		}
	}

	if(modo==modos::fadeout)
	{
		if(fade < 254.0f)
		{
			fade+=delta*150.0f;
			if(fade > 254.0f) fade=254.f;

			int ffade=ceil(fade);
			layout.obtener_por_id("fader")->establecer_alpha(ffade);

			if(fade==254.f)
			{
				modo=modos::fadein;

				switch(indice_menu)
				{
					case 0:	solicitar_cambio_estado(principal); break;
					case 1:	solicitar_cambio_estado(controles); break;
					case 2:	abandonar_aplicacion(); break;
				}
			}
		}
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
			layout.obtener_por_id("flores")->ir_a(140, y);
		}
	}
}
