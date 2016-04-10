#ifndef CONTROLADOR_PRINCIPAL_H
#define CONTROLADOR_PRINCIPAL_H

#include <vector>
#include <memory>
#include <algorithm>

#include <def_video.h>

#include <class/compositor_vista.h>
#include <herramientas/log_base/log_base.h>
#include <class/valor_limitado.h>

#include "../app/representador.h"
#include "../app/localizador.h"
#include "../app/fuentes.h"
#include "../app/mapa.h"
#include "../app/tiempo.h"
#include "../app/jugador.h"
#include "../app/particula.h"
#include "../app/bloque_input.h"
#include "../app/sistema_audio.h"

#include "estados_controladores.h"
#include "../framework/controlador_interface.h"

namespace App
{

class Controlador_principal:
	public DFramework::Controlador_interface
{
	public:

					Controlador_principal(DLibH::Log_base&, const Fuentes&, const Localizador&, Sistema_audio&);

	virtual void 			preloop(DFramework::Input& input, float delta);
	virtual void 			loop(DFramework::Input& input, float delta);
	virtual void 			postloop(DFramework::Input& input, float delta);
	virtual void 			dibujar(DLibV::Pantalla& pantalla);
	virtual void 			despertar();
	virtual void 			dormir();
	virtual bool			es_posible_abandonar_estado() const;

	Mapa&				acc_mapa() {return mapa;}
	void				iniciar_juego();
	void				iniciar_nivel(int, int);
	bool				es_juego_finalizado() const {return juego_finalizado;}
	void				establecer_ayuda(bool v) {info_juego.ayuda_activa=v;}

	private:

	void					ajustar_camara(float delta);

	void					procesar_jugador(DFramework::Input&, float, Jugador&);
	void					procesar_interruptores(float);
	void					procesar_ayudas(float);
	void					procesar_estructuras(float);
	void					procesar_particulas(float);
	void					procesar_florecimiento(float);

	Bloque_input				obtener_bloque_input(DFramework::Input& input) const;

	void					jugador_en_salida(const Salida&, Jugador&); 
	void					jugador_en_interruptor(Interruptor&, Jugador&);
	void					jugador_en_pieza(const Pieza&, Jugador&);
	void					jugador_en_arbol(Arbol&, Jugador&);
	void					jugador_en_ayuda(Ayuda&, Jugador&);
	void					jugador_en_mejora_velocidad(const Mejora_velocidad&, Jugador&);

	void					abrir_puerta(int);
	void					chocar_jugador(Jugador&);
	void					preparar_confirmar_salida();
	void					asignar_mensaje(const std::string&);
	void					centrar_mensaje();
	void					crear_brillo(Espaciable::tpunto);
	bool					hay_input_jugador(DFramework::Input&) const;

	DLibH::Log_base&			log;
	const DLibV::Fuente_TTF&		fuente;
	const DLibV::Fuente_TTF&		fuente_hud;
	const Localizador&			localizador;
	Sistema_audio&				sistema_audio;
	Representador				representador;

	std::vector<Info_audio_reproducir>	reproducir;
	std::vector<Audio_detener>		detener;

	enum class				modos{juego, ayuda, confirmar_salida, animacion_choque, florecimiento, recuento_final} modo;

	DLibV::Camara				camara;
	Mapa					mapa;
	Jugador					jugador;
	std::vector<std::unique_ptr<Particula>>	particulas;
	Tiempo					tiempo;
	Herramientas_proyecto::Compositor_vista		layout_mensaje;
	bool					juego_finalizado;

	struct
	{
		int				id_mapa;
		Inicio				inicio_actual;

	}info_mapa{1, Inicio(Espaciable::tpunto{0.0,0.0}, 1, 0)};

	struct info_interruptor
	{
		size_t				total,
						actual;
		int				id_puerta;
		Herramientas_proyecto::Valor_limitado<float>	tiempo;

		void 				activar(int val)
		{
			++actual;
			tiempo=(float)val/1000.f;
		}
	
		bool 				es_completo() const {return total==actual;}
		void				finalizar()
		{
			tiempo=0.0f;
		}

		void 				turno(float delta)
		{
			if(tiempo > 0.0f)
			{
				tiempo-=delta;

				if(tiempo==0.0f)
				{
					actual=0;
				}
			}
		}

		info_interruptor(int t=0, int a=0, int id=0): total(t), actual(a), id_puerta(id), tiempo(0.0f, -100.0f, 0.0f, Herramientas_proyecto::Valor_limitado<float>::inferior) {}
	};
	std::map<int, info_interruptor>		info_interruptores;

	struct persistente
	{
		std::vector<int>		puertas_abiertas;
		std::vector<int>		piezas_recogidas;

		void				reiniciar()
		{
			puertas_abiertas.clear();
			piezas_recogidas.clear();
		}

		template<typename T>
		void helper_int(std::vector<T>& vec, int v)
		{
			if(std::none_of(std::begin(vec), std::end(vec), [v](int val) {return val==v;})) vec.push_back(v);
		}

		void				abrir_puerta(int v) {helper_int(puertas_abiertas, v);}
		void				recoger_pieza(int v) {helper_int(piezas_recogidas, v);}

	}info_persistente;

	struct
	{
		bool				ayuda_activa=true;
	}info_juego;
};

}

#endif
