#include "controles.h"

#include <vector>

#include "estados_controladores.h"
#include "../app/framework_impl/input.h"

#ifdef WINCOMPIL
/* Localización del parche mingw32... Esto debería estar en otro lado, supongo. */
#include <herramientas/herramientas/herramientas.h>
#endif

using namespace App;

Controlador_controles::Controlador_controles(DLibH::Log_base& log, App_config& c, const Fuentes& f, const Localizador& loc, const DFramework::Input& i)
	:log(log), config(c), 
	fuente(f.obtener_fuente("imagination_station", 16)), 
	localizador(loc)
{
	preparar_controles(i);

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

	if(input.es_input_down(Input::escape))
	{
		solicitar_cambio_estado(principal);
		return;
	}

	auto c=input.obtener_entrada();
	using E=DFramework::Input::Entrada;

	if(c.tipo!=E::ttipo::nada)
	{
//		input.limpiar(kactual);

//		controles[kactual].entrada=c;
//		input.configurar(input.desde_entrada(c, kactual));

//		++kactual;

//		if(!controles.count(kactual))
//		{
//			finalizar_configuracion();
//		}
//		else
//		{
//			recomponer_str_controles();
//			asignar_str_control_actual();
//		}
	}
}

void  Controlador_controles::postloop(DFramework::Input& input, float delta)
{

}

void  Controlador_controles::dibujar(DLibV::Pantalla& pantalla)
{
	layout.volcar(pantalla);
/*
	DLibV::Representacion_TTF txt(fuente, {255, 255, 255, 255}, str_actual);
	txt.ir_a(16, 16);
	txt.volcar(pantalla);

	DLibV::Representacion_TTF txt2(fuente, {255, 255, 255, 255}, str_controles);
	txt2.ir_a(16, 64);
	txt2.volcar(pantalla);
*/
}

void  Controlador_controles::despertar()
{
	layout.parsear("data/layout/layout_controles.dnot", "layout");
}

void  Controlador_controles::dormir()
{
	layout.vaciar_vista();
}

bool Controlador_controles::es_posible_abandonar_estado() const
{
	return true;
}

void Controlador_controles::preparar_controles(const DFramework::Input& input)
{
/*	
	controles[Input::izquierda]={1, tinput::izquierda, Input::izquierda, "P1: Left", input.localizar_entrada(Input::izquierda)};
	controles[Input::derecha]={1, tinput::derecha, Input::derecha, "P1: Right", input.localizar_entrada(Input::j1_derecha)};
	controles[Input::arriba]={1, tinput::arriba, Input::arriba, "P1: Forward", input.localizar_entrada(Input::j1_arriba)};
	controles[Input::abajo]={1, tinput::abajo, Input::abajo, "P1: Backward", input.localizar_entrada(Input::j1_abajo)};
*/
}

std::string Controlador_controles::traducir_input(const DFramework::Input::Entrada& e) const
{
	std::string res="???";

	//TODO: Add locale.
	switch(e.tipo)
	{
		case DFramework::Input::Entrada::ttipo::nada: break;

		case DFramework::Input::Entrada::ttipo::teclado: 
			res="Keyboard ("+std::string(SDL_GetKeyName(SDL_GetKeyFromScancode((SDL_Scancode)e.codigo)))+")";
		break;

		case DFramework::Input::Entrada::ttipo::raton: 
			res="Mouse button "+std::to_string(e.codigo);
		break;

		case DFramework::Input::Entrada::ttipo::joystick: 
			res="Joystick ["+std::to_string(e.dispositivo)+"] button "+std::to_string(e.codigo);
		break;
	}

	return res;
}

void Controlador_controles::finalizar_configuracion()
{
	//TODO: Esto casi que puede ser aprovechable.

//	for(const auto& par : controles)
//	{
//		const auto& c=par.second;
//		App_config::input_jugador val={App_config::input_fw_a_config(c.entrada.tipo), c.entrada.dispositivo, c.entrada.codigo};

/*
		//TODO: Esto tiene que ser una función diferente...
		switch(c.tipo)
		{
			case tinput::habilidad: 	config.mut_habilidad(c.jugador, val); break;
			case tinput::disparo: 		config.mut_disparo(c.jugador, val); break;
			case tinput::arriba:		config.mut_arriba(c.jugador, val); break;
			case tinput::abajo:		config.mut_abajo(c.jugador, val); break;
			case tinput::izquierda:		config.mut_izquierda(c.jugador, val); break;
			case tinput::derecha:		config.mut_derecha(c.jugador, val); break;
		}
*/
//	}

	config.grabar();
	solicitar_cambio_estado(principal);
}
