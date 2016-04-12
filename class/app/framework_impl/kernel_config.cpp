#include "kernel_config.h"
#include <SDL2/SDL.h>
#include "input.h"

#include <iostream>

using namespace App;

Kernel_config::Kernel_config(const App_config& c)
	:config(c)
{

}

std::vector<std::string> Kernel_config::obtener_entradas_audio() const
{
	return obtener_entradas_desde_ruta(std::string("data/recursos/audio.txt"));
}

std::vector<std::string> Kernel_config::obtener_entradas_musica() const
{
	return obtener_entradas_desde_ruta(std::string("data/recursos/musica.txt"));
}

std::vector<std::string> Kernel_config::obtener_entradas_texturas() const
{
	return obtener_entradas_desde_ruta(std::string("data/recursos/texturas.txt"));
}

std::vector<std::string> Kernel_config::obtener_entradas_superficies() const
{
	return obtener_entradas_desde_ruta(std::string("data/recursos/superficies.txt"));
}

DFramework::Info_ventana Kernel_config::obtener_info_ventana() const
{
	return DFramework::Info_ventana{800, 500, 800, 500, "Go : Familiar Game Jam 6", true};
}

std::vector<DFramework::Par_input> Kernel_config::obtener_pares_input() const
{
	auto tipo_desde_config=[](int t)
	{
		using namespace DFramework;

		switch(t)
		{
			case 0: return Par_input::tipos::teclado; break;
			case 1: return Par_input::tipos::joystick; break;
		}

		return Par_input::tipos::teclado;
	};

	using namespace DFramework;
	std::vector<Par_input> res{
		Par_input{Par_input::tipos::teclado, Input::escape, SDL_SCANCODE_ESCAPE, 0},
		Par_input{Par_input::tipos::teclado, Input::tecla_1, SDL_SCANCODE_1, 0},
		Par_input{Par_input::tipos::teclado, Input::tecla_2, SDL_SCANCODE_2, 0},
		Par_input{Par_input::tipos::teclado, Input::tecla_3, SDL_SCANCODE_3, 0},
		Par_input{Par_input::tipos::teclado, Input::tecla_4, SDL_SCANCODE_4, 0},
		Par_input{Par_input::tipos::teclado, Input::tab, SDL_SCANCODE_TAB, 0},
		Par_input{Par_input::tipos::teclado, Input::enter, SDL_SCANCODE_RETURN, 0},
		Par_input{Par_input::tipos::teclado, Input::ayuda, SDL_SCANCODE_F1, 0},
		Par_input{Par_input::tipos::teclado, Input::cambio_logica, SDL_SCANCODE_F2, 0},
		Par_input{Par_input::tipos::teclado, Input::zoom_mas, SDL_SCANCODE_KP_PLUS, 0},
		Par_input{Par_input::tipos::teclado, Input::zoom_menos, SDL_SCANCODE_KP_MINUS, 0},
		Par_input{Par_input::tipos::teclado, Input::reset_zoom, SDL_SCANCODE_0, 0},
		Par_input{Par_input::tipos::teclado, Input::grid_mas, SDL_SCANCODE_RIGHTBRACKET, 0},
		Par_input{Par_input::tipos::teclado, Input::grid_menos, SDL_SCANCODE_LEFTBRACKET, 0},
		Par_input{Par_input::tipos::teclado, Input::profundidad_mas, SDL_SCANCODE_M, 0},
		Par_input{Par_input::tipos::teclado, Input::profundidad_menos, SDL_SCANCODE_N, 0},
		Par_input{Par_input::tipos::teclado, Input::seleccion_color, SDL_SCANCODE_C, 0},
		Par_input{Par_input::tipos::teclado, Input::propiedades_mapa, SDL_SCANCODE_F3, 0},
		Par_input{Par_input::tipos::teclado, Input::intercambiar_debug, SDL_SCANCODE_F3, 0},
		Par_input{Par_input::tipos::teclado, Input::copiar, SDL_SCANCODE_C, 0},
		Par_input{Par_input::tipos::teclado, Input::pegar, SDL_SCANCODE_V, 0},
		Par_input{Par_input::tipos::teclado, Input::intercambiar_decoracion, SDL_SCANCODE_F, 0},
		Par_input{Par_input::tipos::teclado, Input::cargar_mapa, SDL_SCANCODE_F5, 0},
		Par_input{Par_input::tipos::teclado, Input::grabar_mapa, SDL_SCANCODE_F9, 0},
		Par_input{Par_input::tipos::teclado, Input::cursor_izquierda, SDL_SCANCODE_LEFT, 0},
		Par_input{Par_input::tipos::teclado, Input::cursor_derecha, SDL_SCANCODE_RIGHT, 0},
		Par_input{Par_input::tipos::teclado, Input::cursor_arriba, SDL_SCANCODE_UP, 0},
		Par_input{Par_input::tipos::teclado, Input::cursor_abajo, SDL_SCANCODE_DOWN, 0},
		Par_input{Par_input::tipos::teclado, Input::suprimir, SDL_SCANCODE_DELETE, 0},
		Par_input{Par_input::tipos::teclado, Input::control_izquierdo, SDL_SCANCODE_LCTRL, 0},
		Par_input{Par_input::tipos::raton, Input::click_i, SDL_BUTTON_LEFT, 0},
		Par_input{Par_input::tipos::raton, Input::click_d, SDL_BUTTON_RIGHT, 0},

		Par_input{Par_input::tipos::teclado, Input::menu_arriba, SDL_SCANCODE_UP, 0},
		Par_input{Par_input::tipos::teclado, Input::menu_abajo, SDL_SCANCODE_DOWN, 0},
		Par_input{Par_input::tipos::teclado, Input::menu_ok, SDL_SCANCODE_RETURN, 0},
		Par_input{Par_input::tipos::teclado, Input::menu_ok, SDL_SCANCODE_SPACE, 0}
};


	auto obtener_info_jugador=[this](std::map<int, App_config::input_jugador>& res, int j, int iizq, int ider, int iarr, int iaba, int imapa)
	{
		res[iizq]=config.acc_izquierda(j);
		res[ider]=config.acc_derecha(j);
		res[iarr]=config.acc_arriba(j);
		res[iaba]=config.acc_abajo(j);
		res[imapa]=config.acc_mapa(j);
	};

	std::map<int, App_config::input_jugador> mapa;
	obtener_info_jugador(mapa, 1, Input::izquierda, Input::derecha, Input::arriba, Input::abajo, Input::mapa);

	for(const auto& p : mapa)
		res.push_back({tipo_desde_config(p.second.tipo), p.first, p.second.codigo, p.second.device});
	
	return res;
}
