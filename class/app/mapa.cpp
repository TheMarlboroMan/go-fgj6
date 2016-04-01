#include "mapa.h"

#include <algorithm>

using namespace App;

void Mapa::limpiar()
{
	obstaculos.clear();
	decoraciones.clear();
	inicios.clear();
	salidas.clear();
	piezas.clear();
	interruptores.clear();
	puertas.clear();
	mejoras_velocidad.clear();
	decoraciones_fondo.clear();
	decoraciones_frente.clear();
}

void Mapa::inicializar()
{
	for(const auto& d : decoraciones)
	{
		if(d.es_frente()) decoraciones_frente.push_back(&d);
		else decoraciones_fondo.push_back(&d);
	}

	std::sort(std::begin(decoraciones_frente), std::end(decoraciones_frente), [](const Decoracion * a, const Decoracion * b) {return *a < *b;});
	std::sort(std::begin(decoraciones_fondo), std::end(decoraciones_fondo), [](const Decoracion * a, const Decoracion * b) {return *a < *b;});
}

void Mapa::establecer_info_camara(int minx, int maxx, int miny, int maxy)
{
	info_camara=Mapa_info_camara{minx, maxx, miny, maxy};
}

