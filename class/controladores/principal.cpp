#include "principal.h"

#include <algorithm>

#include <class/generador_numeros.h>

#include "../app/representador.h"

#include "../app/importador.h"

#include "../app/framework_impl/input.h"
#include "estados_controladores.h"

#ifdef WINCOMPIL
/* Localización del parche mingw32... Esto debería estar en otro lado, supongo. */
#include <herramientas/herramientas/herramientas.h>
#endif

using namespace App;

Controlador_principal::Controlador_principal(DLibH::Log_base& log, const Fuentes& f)
	:log(log),
	fuente_akashi(f.obtener_fuente("akashi", 16)),
	camara(0, 0, 800, 500)
{

}

void Controlador_principal::preloop(DFramework::Input& input, float delta)
{

}

void Controlador_principal::loop(DFramework::Input& input, float delta)
{
	if(input.es_senal_salida())
	{
		abandonar_aplicacion();
		return;
	}

	if(input.es_input_down(Input::cambio_logica))
	{
		solicitar_cambio_estado(editor);
		return;
	}

	procesar_jugador(input, delta, jugador);
}

void  Controlador_principal::postloop(DFramework::Input& input, float delta)
{

}

void  Controlador_principal::dibujar(DLibV::Pantalla& pantalla)
{
	pantalla.limpiar(0, 0, 0, 255);

	Representador r;
	ajustar_camara();

	for(const auto& d : mapa.decoraciones_fondo)	d->dibujar(r, pantalla, camara);
	jugador.dibujar(r, pantalla, camara);
	for(const auto& d : mapa.decoraciones_frente)	d->dibujar(r, pantalla, camara);
}

void  Controlador_principal::despertar()
{
	log<<"Inicializando mapa..."<<std::endl;
	mapa.inicializar();

	info_mapa.inicio_actual=mapa.inicios[0];
	jugador.establecer_inicio(info_mapa.inicio_actual.acc_punto(), info_mapa.inicio_actual.acc_angulo());
}

void  Controlador_principal::dormir()
{
	mapa.limpiar();
}

bool Controlador_principal::es_posible_abandonar_estado() const
{
	return true;
}

////////////////

Bloque_input Controlador_principal::obtener_bloque_input(DFramework::Input& input) const
{
	Bloque_input res{0, 0, false};

	if(input.es_input_down(Input::arriba)) res.aceleracion=1;
	else if(input.es_input_down(Input::abajo)) res.aceleracion=-1;

	if(input.es_input_pulsado(Input::izquierda)) res.giro=1;
	else if(input.es_input_pulsado(Input::derecha)) res.giro=-1;

	return res;
}

void Controlador_principal::ajustar_camara()
{
	const int w_pantalla=camara.acc_pos_w(), 
		h_pantalla=camara.acc_pos_h(),
		mitad_w_pantalla=w_pantalla / 2,
		mitad_h_pantalla=h_pantalla / 2;


	int xmin=jugador.acc_poligono().acc_centro().x, xmax=xmin,
		ymin=jugador.acc_poligono().acc_centro().y, ymax=ymin;

/*		//Vamos a determinar cuál es el punto central de todos los actores
		//de juego...
		for(const auto&j : jugadores)
		{
			const auto& c=j.acc_poligono().acc_centro();
			if(c.x < xmin) xmin=c.x;
			else if(c.x > xmax) xmax=c.x;

			if(c.y < ymin) ymin=c.y;
			else if(c.y > ymax) ymax=c.y;
		}

		for(const auto& bot : bots)
		{
			const auto& c=bot.acc_poligono().acc_centro();
			if(c.x < xmin) xmin=c.x;
			else if(c.x > xmax) xmax=c.x;

			if(c.y < ymin) ymin=c.y;
			else if(c.y > ymax) ymax=c.y;
		}		
*/
		//Trataremos ahora el zoom. Vamos a medir la distancia máxima en 
		//x e y que hay entre actores.
	
	const double distx=xmax-xmin, disty=ymax-ymin;

	//Calcular el zoom. Realmente no tengo ni idea, he ido probando
	//hasta que ha funcionado :P.
	const double 	zoomx=distx < mitad_w_pantalla ? 1.0 : (double)mitad_w_pantalla / distx,
			zoomy=disty < mitad_h_pantalla ? 1.0 : (double)mitad_h_pantalla / disty;

	//Nos quedaremos con el menor de los dos factores
	//y nos aseguramos de no acercarnos más del zoom por defecto.
	double fin_zoom=zoomx < zoomy ? 1.0 / zoomx : 1.0 / zoomy;

	if(fin_zoom < 1.0) fin_zoom=1.0;
	camara.mut_zoom(fin_zoom);

	//Este es el punto central...
	int 	centro_x=( xmin + ( (xmax-xmin) / 2) ),
		centro_y=( ymin + ( (ymax-ymin) / 2) );

	//Y a partir del mismo podemos calcular el offset de la cámara, 
	//al que aplicamos el zoom.
	int 	cam_x=centro_x - (mitad_w_pantalla * fin_zoom) ,
		cam_y=-(centro_y + (mitad_h_pantalla * fin_zoom));

/*
TODO
	if(cam_x < -850) cam_x=-860;
	if(cam_y < -120) cam_y=-120;

	//TODO: Antes de enfocar comprobar que no nos salimos por la derecha por el zoom!!!!.
	double ancho=((double)camara.acc_foco_w() * camara.acc_zoom());
	double alto=((double)camara.acc_foco_h() * camara.acc_zoom());

	if(ancho + cam_x > 650 || alto + cam_y > 450) 
	{
		//std::cout<<"FUERA CAMARA!!!"<<std::endl;
	}
*/

	//Y finalmente podemos enfocar.
	camara.enfocar_a(cam_x, cam_y);
}

void Controlador_principal::procesar_jugador(DFramework::Input& input, float delta, Jugador &j)
{
	auto bl=obtener_bloque_input(input);
	j.recibir_input(bl);
	j.turno(delta);

	bool colision=false;

	for(const auto& s : mapa.salidas)
	{
		if(j.en_colision_con(s))
		{
			jugador_en_salida(s);
			return;
		}
	}

	for(const auto& o : mapa.obstaculos)
	{
		if(j.en_colision_con(o))
		{
			switch(o.acc_tipo())
			{
				case Obstaculo::ttipos::normal: j.colisionar(true); break;
				case Obstaculo::ttipos::inocuo: j.colisionar(false); break;
			}
			colision=true;
			break;
		}
	}

	if(colision)
	{
		//TODO...
	}
}

void Controlador_principal::jugador_en_salida(const Salida& s)
{
	//
}
