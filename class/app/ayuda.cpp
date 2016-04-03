#include "ayuda.h"

#include <video/representacion/representacion_grafica/representacion_bitmap/representacion_bitmap.h>
#include <video/gestores/gestor_texturas.h>

using namespace App;

Ayuda::Ayuda(Espaciable::tpunto pt, int indice)
	:indice(indice), tiempo_activo(0.0f, 0.0f, 0.0f, Herramientas_proyecto::Valor_limitado<float>::inferior)
{
	formar_poligono();
	establecer_posicion(pt.x, pt.y);
}

void Ayuda::formar_poligono()
{
	poligono.insertar_vertice({-15.0, 15.0});
	poligono.insertar_vertice({15.0, 15.0});
	poligono.insertar_vertice({15.0, -15.0});
	poligono.insertar_vertice({-15.0, -15.0});
	poligono.cerrar();
	poligono.mut_centro({0.0, 0.0});
}

void Ayuda::dibujar(Representador& r, DLibV::Pantalla& pantalla, const DLibV::Camara& camara) const
{
	auto c=poligono.acc_centro();

	DLibV::Representacion_bitmap flare(DLibV::Gestor_texturas::obtener(1));
	flare.establecer_modo_blend(DLibV::Representacion::BLEND_ALPHA);
	flare.establecer_alpha(255);
	flare.ir_a(c.x - 25, -c.y - 25);
	flare.volcar(pantalla, camara);

	DLibV::Representacion_bitmap sprite(DLibV::Gestor_texturas::obtener(4));
	sprite.establecer_modo_blend(DLibV::Representacion::BLEND_ALPHA);
	sprite.establecer_alpha(128);
	sprite.establecer_recorte(105, 30, 30, 30);
	sprite.establecer_posicion(c.x-15, -c.y-15, 30, 30);
	sprite.transformar_centro_rotacion(15 / camara.acc_zoom(), 15 / camara.acc_zoom());
	sprite.volcar(pantalla, camara);
}

void Ayuda::turno(float delta)
{
	if(tiempo_activo > 0.f)
	{
		tiempo_activo-=delta;
	}
}
