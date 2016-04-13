#include "hoja_arbol.h"

#include <video/representacion/representacion_grafica/representacion_bitmap/representacion_bitmap.h>
#include <video/gestores/gestor_texturas.h>
#include <class/generador_numeros.h>

#include "recursos.h"

using namespace App;

Hoja_arbol::Hoja_arbol(Espaciable::tpunto c, double ang, float vel, float t)
	:Particula(c, ang, vel), tiempo(0.0f, t, t, Herramientas_proyecto::Valor_limitado<float>::inferior)
{
	Herramientas_proyecto::Generador_int g(0, 3);
	rep=g();
}

void Hoja_arbol::turno(float delta)
{
	tiempo-=delta;

	DLibH::Vector_2d<double> v=vector_unidad_para_angulo_cartesiano(angulo);
	DLibH::Punto_2d<double> pd{v.x * (velocidad*delta), v.y * (velocidad*delta)};
	centro+=pd;
}

void Hoja_arbol::dibujar(Representador& r, DLibV::Pantalla& pantalla, const DLibV::Camara& camara) const
{
	DLibV::Representacion_bitmap sprite(DLibV::Gestor_texturas::obtener(r_graficos::g_sprites));
	sprite.establecer_modo_blend(DLibV::Representacion::BLEND_ALPHA);

	float alpha=255.0f - ( (tiempo) / 255.f);;

	sprite.establecer_alpha((int)alpha);
	sprite.establecer_recorte(60, 70+(rep*10), 10, 10);
	sprite.establecer_posicion(centro.x-5, -centro.y-5, 10, 10);
	sprite.transformar_centro_rotacion(5 / camara.acc_zoom(), 5 / camara.acc_zoom());
	sprite.transformar_rotar(angulo+(tiempo * 100.f));
	sprite.volcar(pantalla, camara);
}
