#include "mejora_velocidad.h"

#include <video/representacion/representacion_grafica/representacion_bitmap/representacion_bitmap.h>
#include <video/gestores/gestor_texturas.h>

#include "recursos.h"

using namespace App;

Mejora_velocidad::Mejora_velocidad(Espaciable::tpunto pt, int nivel)
	:nivel(nivel), tiempo(0.0f)
{
	formar_poligono();
	establecer_posicion(pt.x, pt.y);
}

void Mejora_velocidad::turno(float delta)
{
	tiempo+=delta;
	if(tiempo > 0.05) tiempo=0.0f;
}

void Mejora_velocidad::formar_poligono()
{
	poligono.insertar_vertice({-15.0, 15.0});
	poligono.insertar_vertice({15.0, 15.0});
	poligono.insertar_vertice({15.0, -15.0});
	poligono.insertar_vertice({-15.0, -15.0});
	poligono.cerrar();
	poligono.mut_centro({0.0, 0.0});
}

void Mejora_velocidad::dibujar(Representador& r, DLibV::Pantalla& pantalla, const DLibV::Camara& camara) const
{
	auto c=poligono.acc_centro();

	DLibV::Representacion_bitmap sprite(DLibV::Gestor_texturas::obtener(r_graficos::g_sprites));
	sprite.establecer_modo_blend(DLibV::Representacion::BLEND_ALPHA);
	sprite.establecer_alpha(255);
	sprite.establecer_recorte(65, 30, 30, 30);
	sprite.establecer_posicion(c.x-15, -c.y-15, 30, 30);
	sprite.transformar_centro_rotacion(15 / camara.acc_zoom(), 15 / camara.acc_zoom());
	sprite.volcar(pantalla, camara);
}
