#include "intro.h"

#include "../app/framework_impl/input.h"
#include "../app/localizacion.h"
#include "../app/recursos.h"
#include "../app/definiciones.h"

using namespace App;

Controlador_intro::Controlador_intro(DLibH::Log_base& log, const Fuentes& fuentes, const Localizador& loc, Sistema_audio& sa)
	:log(log), fuente(fuentes.obtener_fuente("imagination_station", 16)),
	localizador(loc), sistema_audio(sa), juego_finalizado(false)
{
	componente_menu.crear_menu_opciones(env::data_path+"/data/config/valores.dnot", "config_intro", localizador);

	layout.mapear_fuente("fuente", fuente);
	layout.mapear_textura("cover", DLibV::Gestor_texturas::obtener(r_graficos::g_cover));
	layout.mapear_textura("flores", DLibV::Gestor_texturas::obtener(r_graficos::g_flores));
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
	layout.registrar_externa("menu", componente_menu.representacion());
	layout.parsear(env::data_path+"/data/layout/layout_intro.dnot", "layout");

	if(juego_finalizado) layout.obtener_por_id("flores")->hacer_visible();

	static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id("txt_pulsa_tecla"))->asignar(localizador.obtener(Localizacion::intro_pulsa_algo));

	fade.reset(layout.const_float("ini_fade_capa"), layout.const_float("fin_fade_capa"), layout.const_float("fade_salto"));
	fade_out.reset(layout.const_float("fin_fade_capa"), layout.const_float("ini_fade_capa"), layout.const_float("fade_salto"));
	fade_menu.reset(layout.const_float("ini_fade_menu"), layout.const_float("fin_fade_menu"), layout.const_float("fade_menu_salto"));

	componente_menu.mut_x_listado(layout.const_int("x_listado"));
	componente_menu.mut_y_listado(layout.const_int("y_listado"));
	componente_menu.configurar_listado(layout.const_int("h_item"), layout.const_int("h_listado"));

	componente_menu.representacion().establecer_alpha(layout.const_float("ini_fade_menu"));

	inicializar_menu();
	modo=modos::fadein;
}

void Controlador_intro::dormir()
{
	layout.vaciar_vista();
	layout.vaciar_constantes();
	componente_menu.desmontar();
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
				sistema_audio.insertar(Info_audio_reproducir(
					Info_audio_reproducir::t_reproduccion::simple,
					Info_audio_reproducir::t_sonido::repetible,
					r_sonidos::s_campana, 80, 127));

				modo=modos::transicion;
			}
		break;
		case modos::menu:
			if(input.es_input_down(Input::menu_arriba) || input.es_input_down(Input::menu_abajo))
			{
				if(componente_menu.cambiar_item(input.es_input_down(Input::menu_arriba) ? -1 : 1))
				{
					componente_menu.generar_vista_listado();
				}
			}
			else if(input.es_input_down(Input::menu_ok))
			{
				sistema_audio.insertar(Info_audio_reproducir(
					Info_audio_reproducir::t_reproduccion::simple,
					Info_audio_reproducir::t_sonido::repetible,
					r_sonidos::s_viento, 127, 127));
				modo=modos::fadeout;
			}
		break;
		case modos::fade_finalizado:
		{
			const std::string clave=componente_menu.item_actual().clave;
			if(clave=="10_INICIAR") 	solicitar_cambio_estado(principal);
			else if(clave=="20_CONTROLES")	solicitar_cambio_estado(controles);
			else if(clave=="25_CONFIG")	solicitar_cambio_estado(config);
			else if(clave=="30_SALIR")	abandonar_aplicacion();
			return;
		}
		break;

		case modos::transicion:
		case modos::fadeout:
		break;
	}
}

void Controlador_intro::procesar_fade(float delta)
{
	if(modo==modos::transicion)
	{
		if(!fade_menu.es_finalizado())
		{
			fade_menu.turno(delta);
			Uint8 ffade=fade_menu;
			layout.obtener_por_id("txt_pulsa_tecla")->establecer_alpha(255 - ffade);
			componente_menu.representacion().establecer_alpha(ffade);

			if(fade_menu.es_finalizado()) modo=modos::menu;
		}
	}

	if(modo==modos::fadeout)
	{
		if(!fade_out.es_finalizado())
		{
			fade_out.turno(delta);
			layout.obtener_por_id("fader")->establecer_alpha(fade_out);
			if(fade_out.es_finalizado())	modo=modos::fade_finalizado;
		}
	}
	else
	{
		//Fade y animación de salida...
		if(!fade.es_finalizado())
		{
			fade.turno(delta);
			Uint8 ffade=fade;
			layout.obtener_por_id("fader")->establecer_alpha(ffade);

			int y=ffade < 130 ? 130 : ffade;
			layout.obtener_por_id("cover")->ir_a(140, y);
			layout.obtener_por_id("flores")->ir_a(140, y);
		}
	}
}

//Crea la vista completa de menú. Por un lado tenemos la función que se va a usar
//para dibujar y también la lambda que se manda al componente de menú, para
//rellenar el listado del mismo.

void Controlador_intro::inicializar_menu()
{
	using im=item_menu;

	auto ui=[this](const std::string& c){return Uint8(layout.const_int(c));};
	SDL_Color 	color_activo{ui("color_r_activo"), ui("color_g_activo"), ui("color_b_activo"), ui("color_a_activo")},
			color_inactivo{ui("color_r_inactivo"), ui("color_g_inactivo"), ui("color_b_inactivo"), ui("color_a_inactivo")};

	//Definición de función de dibujado del item del listado.
	im::func frep=[this, color_activo, color_inactivo](DLibV::Representacion_agrupada& r, int x, int y, const std::string& nombre, bool actual)
	{
		auto * txt=new DLibV::Representacion_TTF(fuente, actual ? color_activo : color_inactivo, nombre);
		txt->establecer_posicion(x, y);
		r.insertar_representacion(txt);
	};

	componente_menu.traducir_menu_opciones(localizador);

	componente_menu.montar([this, &frep](Herramientas_proyecto::Listado_vertical<im>& l, Herramientas_proyecto::Menu_opciones<std::string>& m, const std::vector<std::string>& v)
	{
		for(const auto& c : v) l.insertar(im{frep, c, m.nombre_opcion(c)});
	});
}
