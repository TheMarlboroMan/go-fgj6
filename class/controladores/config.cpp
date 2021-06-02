#include "config.h"

#include <vector>

#include "estados_controladores.h"
#include "../app/definiciones.h"
#include "../app/localizacion.h"

#ifdef WINCOMPIL
/* Localización del parche mingw32... Esto debería estar en otro lado, supongo. */
#include <templates/parches_compat.h>
#endif

using namespace App;

Controlador_config::Controlador_config(DLibH::Log_base& log, App_config& c, const Fuentes& f, const Localizador& loc)
	:log(log), config(c),
	fuente(f.obtener_fuente("imagination_station", 16)),
	localizador(loc), modo(modos::fadein),
	componente_menu(),
	fader()
{
	crear_menu();
	layout.mapear_fuente("fuente", fuente);
}

void  Controlador_config::preloop(DFramework::Input& input, float delta)
{

}

void Controlador_config::loop(DFramework::Input& input, float delta)
{
	if(input.es_senal_salida())
	{
		abandonar_aplicacion();
		return;
	}

	switch(modo)
	{
		case modos::fadein:
		case modos::fadeout:
			fader.turno(delta);
			componente_menu.representacion().establecer_alpha((int)fader);
			if(fader.es_finalizado())
			{
				if(modo==modos::fadein) modo=modos::config;
				else salir();
			}
		break;
		case modos::config:
			if(input.es_input_down(Input::escape))
			{
				activar_fadeout();
				return;
			}

			if(input.es_input_down(Input::menu_abajo) || input.es_input_down(Input::menu_arriba))
			{
				//Si hay cambios volvemos a generar la vista: el item actual se marca cambiándolo de color y es necesario redibujar.
				if(componente_menu.cambiar_item(input.es_input_down(Input::menu_arriba) ? -1 : 1))
				{
					componente_menu.generar_vista_listado();
				}
			}
			else if(input.es_input_down(Input::menu_ok))
			{
				activar_menu(componente_menu.item_actual());
			}
		break;
	}
}

void  Controlador_config::postloop(DFramework::Input& input, float delta)
{

}

void  Controlador_config::dibujar(DLibV::Pantalla& pantalla)
{
	layout.volcar(pantalla);
	componente_menu.volcar(pantalla);
}

void  Controlador_config::despertar()
{
	layout.registrar_externa("menu", componente_menu.representacion());
	layout.parsear(env::data_path+"/data/layout/layout_config.dnot", "layout");

	componente_menu.mut_x_listado(layout.const_int("x_listado"));
	componente_menu.mut_y_listado(layout.const_int("y_listado"));
	componente_menu.configurar_listado(layout.const_int("h_item"), layout.const_int("h_listado"));
	modo=modos::fadein;

	fader.reset(1.0f, 255.0f, 200.f);

	inicializar_menu();
}

void  Controlador_config::dormir()
{
	layout.vaciar_vista();
	layout.vaciar_constantes();
	componente_menu.desmontar();
}

bool Controlador_config::es_posible_abandonar_estado() const
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////

void Controlador_config::activar_fadeout()
{
	modo=modos::fadeout;
	fader.reset(255.0f, 1.0f, 200.f);
}

void Controlador_config::inicializar_menu()
{
	using im=item_menu;

	auto ui=[this](const std::string& c){return Uint8(layout.const_int(c));};
		SDL_Color color_activo{ui("color_r_activo"), ui("color_g_activo"), ui("color_b_activo"), ui("color_a_activo")},
		color_inactivo{ui("color_r_inactivo"), ui("color_g_inactivo"), ui("color_b_inactivo"), ui("color_a_inactivo")};

	//Definición de función de dibujado del item del listado.
	im::func frep=[this, color_activo, color_inactivo](DLibV::Representacion_agrupada& r, int x, int y, const std::string& clave, const std::string& nombre, const std::string& valor, bool actual)
	{
		auto * txt=new DLibV::Representacion_TTF(fuente, actual ? color_activo : color_inactivo, "");
		txt->establecer_posicion(x, y);
		txt->asignar(nombre+valor);
		r.insertar_representacion(txt);
	};

	componente_menu.traducir_menu_opciones(localizador);

	auto f=[this, &frep](Herramientas_proyecto::Listado_vertical<im>& l, Herramientas_proyecto::Menu_opciones<std::string>& m, const std::vector<std::string>& v)
	{
		for(const auto& c : v) l.insertar(im(frep, c, m.nombre_opcion(c), m.nombre_seleccion(c)));
	};

	componente_menu.montar(f);
}

//Se llama una única vez: crea el menú y asigna los valores.
void Controlador_config::crear_menu()
{
	componente_menu.crear_menu_opciones(env::data_path+"/data/config/valores.dnot", "config_app", localizador);

#ifdef WINCOMPIL
using namespace compat;
#else
using namespace std;
#endif

	const std::string val_ventana=to_string(config.acc_w_fisica_pantalla())+"x"+to_string(config.acc_h_fisica_pantalla());
	componente_menu.menu().asignar_por_valor_templated<std::string>("10_VENTANA", val_ventana);
	componente_menu.menu().asignar_por_valor_templated<bool>("20_AYUDA", config.es_ayuda_activa());
}

void Controlador_config::salir()
{
	config.grabar();
	solicitar_cambio_estado(intro);
}

void Controlador_config::activar_menu(Controlador_config::item_menu& item)
{
	const std::string& clave=item.clave;

	if(clave=="10_VENTANA")
	{
		componente_menu.menu().rotar_opcion(clave, 1);
		auto val=Herramientas_proyecto::explotar(componente_menu.menu().valor_templated<std::string>(clave), 'x');
		config.mut_w_fisica_pantalla(std::atoi(val[0].c_str()));
		config.mut_h_fisica_pantalla(std::atoi(val[1].c_str()));
	}
	else if(clave=="20_AYUDA")
	{
		componente_menu.menu().rotar_opcion(clave, 1);
		config.mut_ayuda_activa(componente_menu.menu().valor_templated<bool>(clave));
	}
	else if(clave=="30_SALIR")
	{
		activar_fadeout();
		return;
	}

	//Refrescar la vista del menú...
	item.valor=componente_menu.menu().nombre_seleccion(clave);
	componente_menu.generar_vista_listado();
}
