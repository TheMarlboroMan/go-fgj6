#include "pieza.h"

using namespace App;

Pieza::Pieza(Espaciable::tpunto pt, int indice)
	:indice(indice)
{
	formar_poligono();
	establecer_posicion(pt.x, pt.y);
}

void Pieza::formar_poligono()
{
	poligono.insertar_vertice({-10.0, 10.0});
	poligono.insertar_vertice({10.0, 10.0});
	poligono.insertar_vertice({10.0, -10.0});
	poligono.insertar_vertice({-10.0, -10.0});
	poligono.cerrar();
	poligono.mut_centro({0.0, 0.0});
}
