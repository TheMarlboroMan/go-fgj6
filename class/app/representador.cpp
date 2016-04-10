#include "representador.h"

#include <video/representacion/representacion_grafica/representacion_bitmap/representacion_bitmap.h>
#include <video/gestores/gestor_texturas.h>

using namespace App;

Representador::Representador()
	:aviso_velocidad_minima{0, 0.0f}
{

}

void Representador::dibujar_poligono(DLibV::Pantalla& pantalla, const DLibH::Poligono_2d<double>& poligono, tcolor color, const DLibV::Camara& camara)
{
	std::vector<DLibV::Representacion_primitiva_poligono::punto> puntos;

	for(const auto& v : poligono.acc_vertices())
		puntos.push_back(cartesiano_a_sdl(v));

	DLibV::Representacion_primitiva_poligono poli(puntos, color.r, color.g, color.b);
	poli.establecer_alpha(color.a);
	poli.volcar(pantalla, camara);
}

void Representador::dibujar_poligono_lineas(DLibV::Pantalla& pantalla, const DLibH::Poligono_2d<double>& poligono, tcolor color, const DLibV::Camara& camara)
{
	std::vector<DLibV::Representacion_primitiva_poligono::punto> puntos;

	for(const auto& v : poligono.acc_vertices())
		puntos.push_back(cartesiano_a_sdl(v));

	DLibV::Representacion_primitiva_poligono_lineas poli(puntos, color.r, color.g, color.b);
	poli.establecer_alpha(color.a);
	poli.volcar(pantalla, camara);
}

void Representador::dibujar_segmento(DLibV::Pantalla& pantalla, const DLibH::Segmento_2d<double>& seg, tcolor color, const DLibV::Camara& camara)
{
	const auto v1=cartesiano_a_sdl(seg.v1);
	const auto v2=cartesiano_a_sdl(seg.v2);

	DLibV::Representacion_primitiva_linea lin(v1.x, v1.y, v2.x, v2.y, color.r, color.g, color.b);
	lin.establecer_alpha(color.a);
	lin.volcar(pantalla, camara);
}

void Representador::dibujar_poligono(DLibV::Pantalla& pantalla, const DLibH::Poligono_2d<double>& poligono, tcolor color)
{
	std::vector<DLibV::Representacion_primitiva_poligono::punto> puntos;
	for(const auto& v : poligono.acc_vertices()) puntos.push_back(cartesiano_a_sdl(v));

	DLibV::Representacion_primitiva_poligono poli(puntos, color.r, color.g, color.b);
	poli.establecer_alpha(color.a);
	poli.volcar(pantalla);
}

void Representador::dibujar_poligono_lineas(DLibV::Pantalla& pantalla, const DLibH::Poligono_2d<double>& poligono, tcolor color)
{
	std::vector<DLibV::Representacion_primitiva_poligono::punto> puntos;
	for(const auto& v : poligono.acc_vertices()) puntos.push_back(cartesiano_a_sdl(v));

	DLibV::Representacion_primitiva_poligono_lineas poli(puntos, color.r, color.g, color.b);
	poli.establecer_alpha(color.a);
	poli.volcar(pantalla);
}

void Representador::dibujar_segmento(DLibV::Pantalla& pantalla, const DLibH::Segmento_2d<double>& seg, tcolor color)
{
	const auto v1=cartesiano_a_sdl(seg.v1);
	const auto v2=cartesiano_a_sdl(seg.v2);

	DLibV::Representacion_primitiva_linea lin(v1.x, v1.y, v2.x, v2.y, color.r, color.g, color.b);
	lin.establecer_alpha(color.a);
	lin.volcar(pantalla);
}

void Representador::dibujar_rejilla(DLibV::Pantalla& pantalla, int grid, tcolor color, double nx, double ny, double zoom, int w, int h)
{
	DLibV::Representacion_primitiva_linea lin(0, 0, 0, 0, color.r, color.g, color.b);
	lin.establecer_alpha(color.a);

	double 	inix=-(fmod(nx, grid)) / zoom, 
		finx=inix + w;

	while(inix < finx)
	{
		lin.establecer_puntos(inix, 0, inix, h);
		lin.volcar(pantalla);
		inix+=(grid / zoom);
	}

	double 	iniy=(fmod(ny, grid)) / zoom,
		finy=iniy + h;

	while(iniy < finy)
	{
		lin.establecer_puntos(0, iniy, w, iniy);
		lin.volcar(pantalla);
		iniy+=(grid / zoom);
	}
}

void Representador::dibujar_poligono_sin_transformar(DLibV::Pantalla& pantalla, const DLibH::Poligono_2d<double>& poligono, tcolor color)
{
	std::vector<DLibV::Representacion_primitiva_poligono::punto> puntos;
	for(const auto& v : poligono.acc_vertices()) puntos.push_back({(int)v.x, (int)v.y});

	DLibV::Representacion_primitiva_poligono poli(puntos, color.r, color.g, color.b);
	poli.establecer_alpha(color.a);
	poli.volcar(pantalla);
}


DLibV::Representacion_primitiva_poligono_base::punto Representador::cartesiano_a_sdl(const DLibH::Punto_2d<double>& pt)
{
	return DLibV::Representacion_primitiva_poligono_base::punto{(int)floor(pt.x), (int)floor(-pt.y)};
}

void Representador::dibujar_hud(DLibV::Pantalla& pantalla, const DLibV::Fuente_TTF& f, const std::string& cad_tiempo, bool aviso_tiempo, int max_vel, int act_vel, int tot_vel)
{

	auto color=aviso_tiempo ? SDL_Color{255, 32, 32, 255} : SDL_Color{255, 255, 255, 255};
	DLibV::Representacion_TTF txt(f, color, cad_tiempo);
	txt.ir_a(16, 70);
	txt.volcar(pantalla);

	DLibV::Representacion_bitmap sprite(DLibV::Gestor_texturas::obtener(6));
	sprite.establecer_modo_blend(DLibV::Representacion::BLEND_ALPHA);

	auto fd=[&pantalla, &sprite](int& act, int max, int xrec, int alpha)
	{
		sprite.establecer_recorte(xrec, 0, 31, 40);
		sprite.establecer_alpha(alpha);

		while(act <= max)
		{
			sprite.establecer_posicion( (act * 33) + 16 , 16, 31, 40);
			sprite.volcar(pantalla);
			++act;
		}
	};

	//Primero la velocidad actual...
	sprite.establecer_recorte(31, 0, 31, 40);
	sprite.establecer_alpha(255);
	int xvel=0;


	//Marcando velocidad actual...
	fd(xvel, act_vel, 31, 255);

	//Aviso de 
	if(aviso_velocidad_minima.tiempo && xvel <= aviso_velocidad_minima.velocidad) 
	{
		int alpha=int( (aviso_velocidad_minima.tiempo * 255) / 1.2f);
		int cp=xvel;
		fd(cp, aviso_velocidad_minima.velocidad, 62, alpha < 128 ? 128 : alpha);
	}

	if(xvel <= max_vel) fd(xvel, max_vel, 0, 128);
	if(xvel <= tot_vel) fd(xvel, tot_vel, 93, 128);
}

void Representador::turno(float delta)
{
	if(aviso_velocidad_minima.tiempo)
	{
		aviso_velocidad_minima.tiempo-=delta;
		if(aviso_velocidad_minima.tiempo < 0.0f) aviso_velocidad_minima.tiempo=0.0f;
	}
}

void Representador::avisar_velocidad_minima(int vel)
{
	aviso_velocidad_minima={vel, 1.2f};
}
