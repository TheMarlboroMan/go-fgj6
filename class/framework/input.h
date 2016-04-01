#ifndef PROYECTO_SDL2_BASE_INPUT_H
#define PROYECTO_SDL2_BASE_INPUT_H

#include <map>
#include <vector>
#include <input/controles_sdl/controles_sdl.h>
#include "kernel_driver_interface.h"

/*Abstrae los sistemas de input usando la clase Controles_SDL. Cada input del
enum de turno se puede asignar a uno o más valores SDLK_xxx, según lo mismo
pueda hacerse con varias teclas, joystick o no. Esta clase es la propietaria
de la instancia de controles_SDL.
*/

namespace DFramework
{

class Input
{
	////////////////////////
	//Definiciones....

	public:

	struct tinput{int sdl_clave, indice_dispositivo;};
	struct Entrada
	{
		enum class ttipo {nada, teclado, raton, joystick};
		ttipo		tipo;
		int		codigo,
				dispositivo;
	};

	typedef DLibI::Controles_SDL::Posicion_raton Posicion_raton;

	public:

	void 			configurar(const std::vector<Par_input>&);
	void 			configurar(Par_input);
	void			limpiar(int);

	/* Todas estas vamos a imaginar que son finales, ok?... */

	void 			turno(); 

	bool 			es_senal_salida() const;

	//Estos son shortcuts para teclas, botones de ratón y botones de joystick.
	bool 			es_input_down(int) const;
	bool 			es_input_up(int) const;
	bool 			es_input_pulsado(int) const;

	bool 			es_tecla_down(int i) const {return controles_sdl.es_tecla_down(i);}
	bool 			es_tecla_up(int i) const {return controles_sdl.es_tecla_up(i);}	
	bool 			es_tecla_pulsada(int i) const {return controles_sdl.es_tecla_pulsada(i);}
	bool 			hay_eventos_teclado_down() const {return controles_sdl.recibe_eventos_teclado_down();}

	bool 			es_boton_up(int p_boton) const {return controles_sdl.es_boton_up(p_boton);}
	bool 			es_boton_down(int p_boton) const {return controles_sdl.es_boton_down(p_boton);}
	bool 			es_boton_pulsado(int p_boton) const {return controles_sdl.es_boton_pulsado(p_boton);}

	bool 			es_joystick_boton_up(int indice, int p_boton) const {return controles_sdl.es_joystick_boton_up(indice, p_boton);}
	bool 			es_joystick_boton_down(int indice, int p_boton) const {return controles_sdl.es_joystick_boton_down(indice, p_boton);}
	bool 			es_joystick_boton_pulsado(int indice, int p_boton) const {return controles_sdl.es_joystick_boton_pulsado(indice, p_boton);}

	int 			obtener_cantidad_joysticks() const {return controles_sdl.acc_cantidad_joysticks();}
	bool			es_nuevo_joystick_conectado() const {return controles_sdl.es_nuevo_joystick_conectado();}

	void			virtualizar_hats_joystick(int indice) {controles_sdl.virtualizar_hats_joystick(indice);}
	void			virtualizar_ejes_joystick(int indice, int threshold) {controles_sdl.virtualizar_ejes_joystick(indice, threshold);}

//	const DLibI::Controles_SDL::Posicion_raton& acc_posicion_raton() const {return controles_sdl.acc_raton().posicion;}
	Posicion_raton 		acc_posicion_raton() const {return controles_sdl.obtener_posicion_raton();}
	DLibI::Controles_SDL& 	acc_controles_sdl() {return controles_sdl;}
	Entrada			obtener_entrada() const;
	Entrada			localizar_entrada(int) const;
	Par_input		desde_entrada(const Entrada&, int);


	void 			iniciar_input_texto() {controles_sdl.iniciar_input_texto();}
	void 			finalizar_input_texto() {controles_sdl.finalizar_input_texto();}
	void 			vaciar_input_texto() {controles_sdl.vaciar_input_texto();}
	const std::string 	acc_input_texto() const {return controles_sdl.acc_input_texto();}
	bool 			es_input_texto_activo() const {return controles_sdl.es_input_texto_activo();}
	bool			es_eventos_input_texto() const {return controles_sdl.recibe_eventos_texto();}

	protected:

	typedef std::multimap<int, tinput> tipo_mapa;	

	//TODO: No es esto lo mismo que Entrada???
	struct Resultado_lookup
	{
		struct valores
		{
			int val, indice;
		};

		enum t_mapa {NADA=0, TECLADO=1, RATON=2, JOYSTICK=3};
		int mapa;
		std::vector<valores> val;
		Resultado_lookup(int tm):mapa(tm) {}
	};

	mutable std::map<int, Resultado_lookup> lookup;
	tipo_mapa 		mapa_teclado;
	tipo_mapa 		mapa_raton;
	tipo_mapa 		mapa_joystick;

	private:

	Resultado_lookup 	obtener(int) const;
	DLibI::Controles_SDL 	controles_sdl;
};

}
#endif
