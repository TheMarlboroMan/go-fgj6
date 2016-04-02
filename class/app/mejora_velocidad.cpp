#include "mejora_velocidad.h"

using namespace App;

Mejora_velocidad::Mejora_velocidad(Espaciable::tpunto pt, int nivel)
	:nivel(nivel)
{
	formar_poligono();
	establecer_posicion(pt.x, pt.y);
}

void Mejora_velocidad::formar_poligono()
{
	poligono.insertar_vertice({-10.0, 10.0});
	poligono.insertar_vertice({10.0, 10.0});
	poligono.insertar_vertice({10.0, -10.0});
	poligono.insertar_vertice({-10.0, -10.0});
	poligono.cerrar();
	poligono.mut_centro({0.0, 0.0});
}

void Mejora_velocidad::dibujar(Representador& r, DLibV::Pantalla& pantalla, const DLibV::Camara& camara) const
{
	//TODO...
	r.dibujar_poligono(pantalla, poligono, {255, 255, 255, 255}, camara);
}
