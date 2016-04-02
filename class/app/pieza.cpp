#include "pieza.h"

#include <video/representacion/representacion_grafica/representacion_bitmap/representacion_bitmap.h>
#include <video/gestores/gestor_texturas.h>

using namespace App;

Pieza::Pieza(Espaciable::tpunto pt, int indice)
	:indice(indice)
{
	formar_poligono();
	establecer_posicion(pt.x, pt.y);
}

void Pieza::formar_poligono()
{
	poligono.insertar_vertice({-15.0, 15.0});
	poligono.insertar_vertice({15.0, 15.0});
	poligono.insertar_vertice({15.0, -15.0});
	poligono.insertar_vertice({-15.0, -15.0});
	poligono.cerrar();
	poligono.mut_centro({0.0, 0.0});
}

void Pieza::dibujar(Representador& r, DLibV::Pantalla& pantalla, const DLibV::Camara& camara) const
{
	auto c=poligono.acc_centro();

	DLibV::Representacion_bitmap bmp(DLibV::Gestor_texturas::obtener(4));
	DLibV::Representacion_bitmap flare(DLibV::Gestor_texturas::obtener(1));

	flare.establecer_modo_blend(DLibV::Representacion::BLEND_ALPHA);
	flare.establecer_alpha(255);
	flare.ir_a(c.x - 25, -c.y - 25);
	flare.volcar(pantalla, camara);

	bmp.establecer_recorte((indice-1) * 30, 0, 30, 30);
	bmp.establecer_posicion(c.x - 15, -c.y - 15, 30, 30);
	bmp.volcar(pantalla, camara);
}
