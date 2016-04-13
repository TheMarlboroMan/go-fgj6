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
#include "../app/pieza_animacion.h"
#include "../app/tiempo.h"
#include "../app/jugador.h"
#include "../app/particula.h"
#include "../app/bloque_input.h"
#include "../app/sistema_audio.h"

#include "estados_controladores.h"
#include "../framework/controlador_interface.h"

/*
	TODO: Colocamos el automapa en el mismo controlador o en un controlador propio???.

	#include "class/app/automapa.h"

	using namespace App;
	Automapa a;
	a.cargar("data/app/mapa.dat");
	bool res=true;
	a.visitar(1);
	a.visitar(2);
	a.visitar(3);
	a.visitar(4);
	for(const auto& s : a.obtener_visitadas())
	{
		std::cout<<"ID: "<<s.id<<" EN "<<s.x<<","<<s.y<<" CON "<<s.w<<"/"<<s.h<<std::endl;		
		std::cout<<"TIENE "<<s.salidas.size()<<" SALIDAS"<<std::endl;
		std::cout<<"TIENE "<<s.marcadores.size()<<" MARCADORES"<<std::endl;
	}
*/

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
	const std::vector<int>&		obtener_salas_descubiertas() const {return info_persistente.mapas_visitados;}
	int				obtener_id_sala_actual() const {return info_mapa.id_mapa;}
	void				iniciar_juego();
	void				iniciar_nivel(int);
	void				cargar_nivel(int);
	bool				es_juego_finalizado() const {return info_juego.juego_finalizado;}
	void				establecer_ayuda(bool v) {info_juego.ayuda_activa=v;}
	void				establecer_debug(bool v) {info_juego.debug_activo=v;}
	void				establecer_editor(bool v) {info_juego.editor_activo=v;}

	private:

	void					ajustar_camara(float delta);
	void					ajustar_parallax();

	void					procesar_jugador(DFramework::Input&, float, Jugador&);
	void					procesar_interruptores(float);
	void					procesar_ayudas(float);
	void					procesar_estructuras(float);
	void					procesar_particulas(float);
	void					procesar_florecimiento(float);
	void					procesar_animacion_pieza(float);

	Bloque_input				obtener_bloque_input(DFramework::Input& input) const;

	void					jugador_en_salida(Salida, Jugador&); 
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
	void					dibujar_debug(DLibV::Pantalla&);

	DLibH::Log_base&			log;
	const DLibV::Fuente_TTF&		fuente;
	const DLibV::Fuente_TTF&		fuente_hud;
	const Localizador&			localizador;
	Sistema_audio&				sistema_audio;
	Representador				representador;

	std::vector<Info_audio_reproducir>	reproducir;
	std::vector<Audio_detener>		detener;

	enum class				modos{juego, ayuda, confirmar_salida, animacion_choque, animacion_pieza, florecimiento, recuento_final} modo;

	DLibV::Camara				camara;
	DLibV::Representacion_bitmap		fondo;
	struct {
		int				max_fondo_x=0,
						max_fondo_y=0;
	}info_parallax;
	Mapa					mapa;
	Jugador					jugador;
	Pieza_animacion				pieza_animacion;
	std::vector<std::unique_ptr<Particula>>	particulas;
	Tiempo					tiempo;	
	Herramientas_proyecto::Compositor_vista		layout_mensaje;
	
	struct
	{
		int				id_mapa;
		Inicio				inicio_actual;

	}info_mapa{1, Inicio(Espaciable::tpunto{0.0,0.0}, 1, 0)};

/** 
Información sobre los interruptores. Por cada grupo se creará un objeto de
este tipo que lleva la cuenta de los interruptores activados en el grupo y de 
la puerta que abren.
Para que no puedas pulsar dos interruptores consecutivos sin parar, se guardan
las coordenadas de cada uno de ellos en un vector y sólo se suma al total 
cuando no está activado antes.	
*/

	struct info_interruptor
	{
		size_t				total;
		int				id_puerta;
		Herramientas_proyecto::Valor_limitado<float>	tiempo;
		std::vector<Espaciable::tpunto>	activados;

		void 				activar(int val, Espaciable::tpunto pt)
		{
			if(std::find(std::begin(activados), std::end(activados), pt)==std::end(activados))
			{
				activados.push_back(pt);
			}

			tiempo=(float)val/1000.f;
		}
	
		bool 				es_completo() const {return total==activados.size();}

		void				finalizar()
		{
			tiempo=0.0f;
			activados.clear();			
		}

		void 				turno(float delta)
		{
			if(tiempo > 0.0f)
			{
				tiempo-=delta;

				if(tiempo==0.0f)
				{
					activados.clear();
				}
			}
		}

		info_interruptor(int t=0, int id=0): total(t), id_puerta(id), tiempo(0.0f, -100.0f, 0.0f, Herramientas_proyecto::Valor_limitado<float>::inferior) {}
	};
	std::map<int, info_interruptor>		info_interruptores; //el intero es id_grupo.

	struct persistente
	{
		std::vector<int>		puertas_abiertas;
		std::vector<int>		piezas_recogidas;
		std::vector<int>		mapas_visitados;

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
		void				visitar_mapa(int v) {helper_int(mapas_visitados, v);}

	}info_persistente;

	struct
	{
		bool				ayuda_activa=true,
						debug_activo=false,
						editor_activo=false,
						juego_finalizado=false;
	}info_juego;
};

}

#endif
