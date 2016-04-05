#ifndef REPRESENTADOR_H
#define REPRESENTADOR_H

#include <video/pantalla/pantalla.h>
#include <video/camara/camara.h>
#include <video/representacion/representacion_primitiva/representacion_primitiva_poligono/representacion_primitiva_poligono.h>
#include <video/representacion/representacion_primitiva/representacion_primitiva_linea/representacion_primitiva_linea.h>
#include <video/representacion/representacion_grafica/representacion_ttf/representacion_ttf.h>
#include <herramientas/poligono_2d/poligono_2d.h>
#include "color.h"


#include "espaciable.h" //Para el cuadrado.

namespace App
{
class Representador
{
	public:

				Representador();
	void			turno(float);

	void 			dibujar_poligono(DLibV::Pantalla&, const DLibH::Poligono_2d<double>&, tcolor, const DLibV::Camara&);
	void 			dibujar_poligono_lineas(DLibV::Pantalla&, const DLibH::Poligono_2d<double>&, tcolor, const DLibV::Camara&);
	void 			dibujar_segmento(DLibV::Pantalla&, const DLibH::Segmento_2d<double>&, tcolor, const DLibV::Camara&);



	void 			dibujar_poligono(DLibV::Pantalla&, const DLibH::Poligono_2d<double>&, tcolor);
	void 			dibujar_poligono_lineas(DLibV::Pantalla&, const DLibH::Poligono_2d<double>&, tcolor);
	void 			dibujar_segmento(DLibV::Pantalla&, const DLibH::Segmento_2d<double>&, tcolor);

	void 			dibujar_poligono_sin_transformar(DLibV::Pantalla&, const DLibH::Poligono_2d<double>&, tcolor);

	void 			dibujar_rejilla(DLibV::Pantalla&, int grid, tcolor, double nx, double ny, double zoom, int w, int h);

	void			dibujar_hud(DLibV::Pantalla&, const DLibV::Fuente_TTF&, const std::string&, bool, int, int, int);

	void			avisar_velocidad_minima(int);

	private:

	DLibV::Representacion_primitiva_poligono_base::punto	cartesiano_a_sdl(const DLibH::Punto_2d<double>& pt);
	
	struct
	{
		int			velocidad;
		float			tiempo;
	}aviso_velocidad_minima;
};

}

#endif
