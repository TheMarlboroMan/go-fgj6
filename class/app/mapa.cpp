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
	arboles.clear();
	ayudas.clear();
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

void Mapa::recoger_pieza(int id_pieza)
{
	auto it=std::remove_if(std::begin(piezas), std::end(piezas), [id_pieza](const Pieza& p) {return p.acc_indice()==id_pieza;});
	piezas.erase(it, std::end(piezas));
}

void Mapa::abrir_puerta(int id_puerta)
{
	auto it=std::remove_if(std::begin(puertas), std::end(puertas), [id_puerta](const Puerta& p) {return p.acc_id()==id_puerta;});
	puertas.erase(it, std::end(puertas));
}

bool Mapa::existe_puerta(int id_puerta) const
{
	return std::any_of(std::begin(puertas), std::end(puertas), [id_puerta](const Puerta&p) {return p.acc_id()==id_puerta;});
}

void Mapa::actualizar_arbol(const std::vector<int>& v)
{
	if(arboles.size())
	{
		auto& arbol=arboles.front();
		for(const auto& p : v) arbol.colocar_pieza(p);
	}
}
