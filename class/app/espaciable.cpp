#include "espaciable.h"

using namespace App;

Espaciable::Espaciable()
{

}

Espaciable::Espaciable(const tpoligono& p)
	:poligono(p)
{

}

void Espaciable::establecer_posicion(double x, double y)
{
	poligono.centrar_en({x, y});
}

void Espaciable::mover(double x, double y)
{
	auto c=poligono.acc_centro();
	poligono.centrar_en({c.x+x, c.y+y});
}


bool Espaciable::en_colision_con(const Espaciable& e) const
{
	return colision_poligono_SAT(poligono, e.poligono);
}

void Espaciable::desplazar_angulo_velocidad(double angulo, double velocidad)
{
	DLibH::Vector_2d<double> v=vector_unidad_para_angulo_cartesiano(angulo);
	DLibH::Punto_2d<double> pd{v.x * velocidad, v.y * velocidad};
	poligono.desplazar(pd);
}
