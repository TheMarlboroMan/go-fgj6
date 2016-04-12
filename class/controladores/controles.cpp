#include "controles.h"

#include <vector>

#include "estados_controladores.h"
#include "../app/localizacion.h"

#ifdef WINCOMPIL
/* Localización del parche mingw32... Esto debería estar en otro lado, supongo. */
#include <herramientas/herramientas/herramientas.h>
#endif

using namespace App;

Controlador_controles::Controlador_controles(DLibH::Log_base& log, App_config& c, const Fuentes& f, const Localizador& loc, const DFramework::Input& i)
	:log(log), config(c), 
	fuente(f.obtener_fuente("imagination_station", 16)), 
	localizador(loc), modo(modos::fadein),
	componente_menu(),
	fader()
{
	crear_menu_opciones(i);
	layout.mapear_fuente("fuente", fuente);
}

void  Controlador_controles::preloop(DFramework::Input& input, float delta)
{

}

void Controlador_controles::loop(DFramework::Input& input, float delta)
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
				if(modo==modos::fadein) modo=modos::seleccion;
				else salir();
			}
		break;
		case modos::seleccion: 	input_seleccion(input); break;
		case modos::entrada:	input_entrada(input); break;
	}
}

void  Controlador_controles::postloop(DFramework::Input& input, float delta)
{

}

void  Controlador_controles::dibujar(DLibV::Pantalla& pantalla)
{
	layout.volcar(pantalla);
//	componente_menu.volcar(pantalla);
}

void  Controlador_controles::despertar()
{
	layout.registrar_externa("menu", componente_menu.representacion());
	layout.parsear("data/layout/layout_controles.dnot", "layout");

	componente_menu.mut_x_listado(layout.const_int("x_listado"));
	componente_menu.mut_y_listado(layout.const_int("y_listado"));
	componente_menu.configurar_listado(layout.const_int("h_item"), layout.const_int("h_listado"));
	modo=modos::fadein;

	fader.reset(1.0f, 255.0f, 200.f);

	generar_vista_menu();
}

void  Controlador_controles::dormir()
{
	layout.vaciar();
	componente_menu.desmontar();
}

bool Controlador_controles::es_posible_abandonar_estado() const
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////

void Controlador_controles::input_seleccion(DFramework::Input& input)
{	
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
		modo=modos::entrada;
		const std::string clave=componente_menu.item_actual().clave;
		if(clave=="100_SALIR")
		{
			activar_fadeout();
			return;
		}
		else
		{
			const std::string nombre_input=localizador.obtener(indice_traduccion_desde_clave(clave));
			auto r=static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id("txt_input"));
			r->asignar(localizador.obtener(Localizacion::controles_introducir)+nombre_input);
			layout.obtener_por_id("txt_input")->hacer_visible();
		}
	}
}

void Controlador_controles::input_entrada(DFramework::Input& input)
{
	if(input.es_input_down(Input::escape))
	{
		modo=modos::seleccion;
		return;
	}

	auto c=input.obtener_entrada();
	using E=DFramework::Input::Entrada;

	if(c.tipo!=E::ttipo::nada)
	{
		const std::string& clave=componente_menu.item_actual().clave;

		//Se hace el cambio en "input".
		int k=indice_input_desde_clave(clave);
		input.limpiar(k);
		input.configurar(input.desde_entrada(c, k));

		//Actualizamos la configuración, para que los nuevos valores se vean en el listado... Convertimos de framework a propio.
		auto en_config=App_config::fw_a_config(c);
		actualizar_configuracion(componente_menu.item_actual().clave, en_config);

		//Actualizamos el valor interno del menú.
		const std::string valor_menu=string_desde_input(en_config);
		componente_menu.menu().asignar_por_valor_string(clave, valor_menu);

		//Actualizamos el valor del listado...
		componente_menu.item_actual().valor=valor_menu; //Este no es el texto que se verá: se transformará luego.
		componente_menu.generar_vista_listado();

		//Volvemos al modo anterior.
		modo=modos::seleccion;
		layout.obtener_por_id("txt_input")->hacer_invisible();
	}
}

void Controlador_controles::activar_fadeout()
{
	modo=modos::fadeout;
	fader.reset(255.0f, 1.0f, 200.f);
}

std::string Controlador_controles::traducir_input(const App_config::input_jugador& e) const
{
#ifdef WINCOMPIL
using namespace parche_mingw;
#else
using namespace std;
#endif
	std::string res=localizador.obtener(Localizacion::controles_desconocido);

	switch(e.tipo)
	{
		case App_config::input_jugador::nada: break;

		case App_config::input_jugador::teclado: 
			res=localizador.obtener(Localizacion::controles_teclado)+std::string(SDL_GetKeyName(SDL_GetKeyFromScancode((SDL_Scancode)e.codigo)));
		break;

		case App_config::input_jugador::raton: 
			res=localizador.obtener(Localizacion::controles_raton)+to_string(e.codigo);
		break;

		case App_config::input_jugador::joystick: 
			res=localizador.obtener(Localizacion::controles_joystick_a)+to_string(e.device)+localizador.obtener(Localizacion::controles_joystick_b)+to_string(e.codigo);
		break;
	}

	return res;
}

App_config::input_jugador Controlador_controles::input_desde_string(const std::string& s) const
{
	//El texto tiene este aspecto #tipo#,#device#,#codigo#
	auto val=Herramientas_proyecto::explotar(s, ',');
	int	tipo=std::atoi(val[0].c_str()), 
		device=std::atoi(val[1].c_str()), 
		codigo=std::atoi(val[2].c_str());

	return App_config::input_jugador{tipo, device, codigo};
}

std::string Controlador_controles::string_desde_input(const App_config::input_jugador& ij) const
{
#ifdef WINCOMPIL
using namespace parche_mingw;
#else
using namespace std;
#endif

	return to_string(ij.tipo)+","+to_string(ij.device)+","+to_string(ij.codigo);
}

//Crea la vista completa de menú. Por un lado tenemos la función que se va a usar 
//para dibujar y también la lambda que se manda al componente de menú, para 
//rellenar el listado del mismo.

void Controlador_controles::generar_vista_menu()
{
	//TODO: ¿Sería posible centrarlos?.
	auto ui=[this](const std::string& c){return Uint8(layout.const_int(c));};
		SDL_Color color_activo{ui("color_r_activo"), ui("color_g_activo"), ui("color_b_activo"), ui("color_a_activo")}, 
		color_inactivo{ui("color_r_inactivo"), ui("color_g_inactivo"), ui("color_b_inactivo"), ui("color_a_inactivo")};

	//Definición de función de dibujado del item del listado.
	item_config_controles::func frep=[this, color_activo, color_inactivo](DLibV::Representacion_agrupada& r, int x, int y, const std::string& clave, const std::string& nombre, const std::string& valor, bool actual)
	{
		auto * txt=new DLibV::Representacion_TTF(fuente, actual ? color_activo : color_inactivo, "");
		txt->establecer_posicion(x, y);
		txt->asignar(clave=="100_SALIR" ? nombre : nombre+"\t\t\t"+traducir_input(input_desde_string(valor)));
		r.insertar_representacion(txt);
	};

	componente_menu.traducir_menu_opciones(localizador);

	auto f=[this, &frep](Herramientas_proyecto::Listado_vertical<item_config_controles>& l, Herramientas_proyecto::Menu_opciones<std::string>& m, const std::vector<std::string>& v)
	{
		for(const auto& c : v) l.insertar(item_config_controles(frep, c, m.nombre_opcion(c), m.nombre_seleccion(c)));
	};

	componente_menu.montar(f);
}

//Se llama una única vez: crea el menú y asigna los valores.
void Controlador_controles::crear_menu_opciones(const DFramework::Input& input)
{
	componente_menu.crear_menu_opciones("data/config/valores.dnot", "config_controles", localizador);

	componente_menu.menu().asignar_por_valor_string("010_K_IZQUIERDA", string_desde_input(config.acc_izquierda(1)));
	componente_menu.menu().asignar_por_valor_string("020_K_DERECHA", string_desde_input(config.acc_derecha(1)));
	componente_menu.menu().asignar_por_valor_string("030_K_ARRIBA", string_desde_input(config.acc_arriba(1)));
	componente_menu.menu().asignar_por_valor_string("040_K_ABAJO", string_desde_input(config.acc_abajo(1)));
	componente_menu.menu().asignar_por_valor_string("050_K_MAPA", string_desde_input(config.acc_mapa(1)));
}

//TODO: Todas estas cosas las podríamos de alguna forma meter en un sólo objeto que se haga cargo??.

void Controlador_controles::actualizar_configuracion(const std::string& c, App_config::input_jugador v)
{
	if(c=="010_K_IZQUIERDA") 	config.mut_izquierda(1, v);
	else if(c=="020_K_DERECHA")	config.mut_derecha(1, v);
	else if(c=="030_K_ARRIBA")	config.mut_arriba(1, v);
	else if(c=="040_K_ABAJO")	config.mut_abajo(1, v);
	else if(c=="050_K_MAPA")		config.mut_mapa(1, v);
}

int Controlador_controles::indice_input_desde_clave(const std::string& c) const
{
	if(c=="010_K_IZQUIERDA") 	return Input::izquierda;
	else if(c=="020_K_DERECHA")	return Input::derecha;
	else if(c=="030_K_ARRIBA")	return Input::arriba;
	else if(c=="040_K_ABAJO")	return Input::abajo;
	else if(c=="050_K_MAPA")		return Input::mapa;
	else return 0;
}

int Controlador_controles::indice_traduccion_desde_clave(const std::string& c) const
{
	if(c=="010_K_IZQUIERDA") 	return Localizacion::controles_izquierda;
	else if(c=="020_K_DERECHA")	return Localizacion::controles_derecha;
	else if(c=="030_K_ARRIBA")	return Localizacion::controles_arriba;
	else if(c=="040_K_ABAJO")	return Localizacion::controles_abajo;
	else if(c=="050_K_MAPA")	return Localizacion::controles_mapa;
	else return 0;
}

void Controlador_controles::salir()
{
	config.grabar();
	solicitar_cambio_estado(intro);
}
