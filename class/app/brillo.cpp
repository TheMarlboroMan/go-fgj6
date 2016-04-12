#include "brillo.h"

#include <video/representacion/representacion_grafica/representacion_bitmap/representacion_bitmap.h>
#include <video/gestores/gestor_texturas.h>
#include <class/generador_numeros.h>

#include "recursos.h"

using namespace App;

Brillo::Brillo(Espaciable::tpunto c, double ang, float vel, float t)
	:Particula(c, ang, vel), tiempo(0.0f, t, t, Herramientas_proyecto::Valor_limitado<float>::inferior)
{
	Herramientas_proyecto::Generador_int g(0, 7);
	rep=g();
}

void Brillo::turno(float delta)
{
	tiempo-=delta;
	DLibH::Vector_2d<double> v=vector_unidad_para_angulo_cartesiano(angulo);
	DLibH::Punto_2d<double> pd{v.x * (velocidad*delta), v.y * (velocidad*delta)};
	centro+=pd;
}

void Brillo::dibujar(Representador& r, DLibV::Pantalla& pantalla, const DLibV::Camara& camara) const
{
	DLibV::Representacion_bitmap sprite(DLibV::Gestor_texturas::obtener(r_graficos::g_sprites));
	sprite.establecer_modo_blend(DLibV::Representacion::BLEND_ALPHA);

	float alpha=255.0f - ( (tiempo) / 255.f);;

	sprite.establecer_alpha((int)alpha);
	sprite.establecer_recorte(60, 30+(rep*5), 5, 5);
	sprite.establecer_posicion(centro.x-2, -centro.y-2, 5, 5);
	sprite.transformar_centro_rotacion(2 / camara.acc_zoom(), 2 / camara.acc_zoom());
	sprite.transformar_rotar(angulo+(tiempo * 300.f));
	sprite.volcar(pantalla, camara);
}
