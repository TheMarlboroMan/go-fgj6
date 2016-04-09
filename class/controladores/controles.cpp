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
	localizador(loc), modo(modos::seleccion),
	//TODO: Especificar medidas por constantes.
	componente_menu(32, 32, 20, 200)
{
	layout.mapear_fuente("fuente", fuente);
	crear_menu_opciones(i);
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
		case modos::seleccion:

			if(input.es_input_down(Input::escape))
			{
				config.grabar();
				solicitar_cambio_estado(intro);
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
				const std::string nombre_input=localizador.obtener(indice_traduccion_desde_clave(componente_menu.item_actual().clave));
				auto r=static_cast<DLibV::Representacion_TTF *>(layout.obtener_por_id("txt_input"));
				r->asignar(localizador.obtener(Localizacion::controles_introducir)+nombre_input);
				layout.obtener_por_id("txt_input")->hacer_visible();
			}
		break;
		
		case modos::entrada:
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
				componente_menu.item_actual().texto=valor_menu;	//Este no es el texto que se verá: se transformará luego.
				componente_menu.generar_vista_listado();

				//Volvemos al modo anterior.
				modo=modos::seleccion;
				layout.obtener_por_id("txt_input")->hacer_invisible();
			}
		}
	}
}

void  Controlador_controles::postloop(DFramework::Input& input, float delta)
{

}

void  Controlador_controles::dibujar(DLibV::Pantalla& pantalla)
{
	layout.volcar(pantalla);
	//TODO: Medidas por constantes.
	componente_menu.volcar(pantalla, 600, 20);
}

void  Controlador_controles::despertar()
{
	layout.parsear("data/layout/layout_controles.dnot", "layout");
	generar_vista_menu();
}

void  Controlador_controles::dormir()
{
	layout.vaciar_vista();
	componente_menu.desmontar();
}

bool Controlador_controles::es_posible_abandonar_estado() const
{
	return true;
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
	int	tipo=std::stoi(val[0].c_str()), 
		device=std::stoi(val[1].c_str()), 
		codigo=std::stoi(val[2].c_str());

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
	//Definición de función de dibujado del item del listado.

	//TODO: ¿Sería posible centrarlos?.
	item_config_controles::func frep=[this](DLibV::Representacion_agrupada& r, int x, int y, const std::string& nombre, const std::string& texto, bool actual)
	{
		SDL_Color color=actual ? SDL_Color{0, 0, 0, 255} : SDL_Color{128, 128, 128, 255};
		auto * txt=new DLibV::Representacion_TTF(fuente, color, texto);
		txt->establecer_posicion(x, y);
		txt->asignar(nombre+"\t\t\t"+traducir_input(input_desde_string(texto)));
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
	componente_menu.crear_menu_opciones("data/config/config_menu.dnot", "config_controles", localizador);

	componente_menu.menu().asignar_por_valor_string("01_K_IZQUIERDA", string_desde_input(config.acc_izquierda(1)));
	componente_menu.menu().asignar_por_valor_string("02_K_DERECHA", string_desde_input(config.acc_derecha(1)));
	componente_menu.menu().asignar_por_valor_string("03_K_ARRIBA", string_desde_input(config.acc_arriba(1)));
	componente_menu.menu().asignar_por_valor_string("04_K_ABAJO", string_desde_input(config.acc_abajo(1)));
}

void Controlador_controles::actualizar_configuracion(const std::string& c, App_config::input_jugador v)
{
	if(c=="01_K_IZQUIERDA") 	config.mut_izquierda(1, v);
	else if(c=="02_K_DERECHA")	config.mut_derecha(1, v);
	else if(c=="03_K_ARRIBA")	config.mut_arriba(1, v);
	else if(c=="04_K_ABAJO")	config.mut_abajo(1, v);
}

int Controlador_controles::indice_input_desde_clave(const std::string& c) const
{
	if(c=="01_K_IZQUIERDA") 	return Input::izquierda;
	else if(c=="02_K_DERECHA")	return Input::derecha;
	else if(c=="03_K_ARRIBA")	return Input::arriba;
	else if(c=="04_K_ABAJO")	return Input::abajo;
	else return 0;
}

int Controlador_controles::indice_traduccion_desde_clave(const std::string& c) const
{
	if(c=="01_K_IZQUIERDA") 	return Localizacion::controles_izquierda;
	else if(c=="02_K_DERECHA")	return Localizacion::controles_derecha;
	else if(c=="03_K_ARRIBA")	return Localizacion::controles_arriba;
	else if(c=="04_K_ABAJO")	return Localizacion::controles_abajo;
	else return 0;
}
