#include "fuentes.h"

using namespace App;

const DLibV::Fuente_TTF& Fuentes::obtener_fuente(const std::string f, int t) const
{
	if(!fuentes.count({f, t}))
	{
		throw std::runtime_error("La fuente "+f+" no está registrada en el tamaño solicitado");
	}

	return fuentes.at({f,t});
}

void Fuentes::registrar_fuente(const std::string f, int t)
{
	if(!fuentes.count({f, t}))
	{
		fuentes.insert( std::pair<info_fuente, DLibV::Fuente_TTF>({f, t}, DLibV::Fuente_TTF("data/fuentes/"+f+".ttf", t) ) );
	}
}
