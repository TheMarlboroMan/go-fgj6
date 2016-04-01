#include "decoracion.h"

using namespace App;

Decoracion::Decoracion(const tpoligono& p, tcolor c, tcolor cl, bool fr, int prof)
	:Espaciable(p), color(c), color_linea(cl), frente(fr), profundidad(prof)
{

}

bool Decoracion::operator<(const Decoracion& o) const
{
	return profundidad < o.profundidad;
}

void Decoracion::dibujar(Representador& r, DLibV::Pantalla& pantalla, const DLibV::Camara& camara) const
{
	r.dibujar_poligono(pantalla, poligono, color, camara);
	if(color_linea!=color) r.dibujar_poligono_lineas(pantalla, poligono, color_linea, camara);
}

void Decoracion::mut_profundidad(int v)
{
	profundidad=v;
	comprobar_validez_profundidad();
}

void Decoracion::subir_profundidad()
{
	++profundidad;
	comprobar_validez_profundidad();
}

void Decoracion::bajar_profundidad()
{
	--profundidad;
	comprobar_validez_profundidad();
}

void Decoracion::comprobar_validez_profundidad()
{
	if(profundidad < min_profundidad) profundidad=min_profundidad;
	else if(profundidad > max_profundidad) profundidad=max_profundidad;
}
