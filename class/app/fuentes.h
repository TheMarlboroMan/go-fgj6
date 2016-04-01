#ifndef FUENTES_H
#define FUENTES_H

#include <map>
#include <stdexcept>

#include <iostream>

#include <video/fuente_ttf/fuente_ttf.h>

namespace App
{

struct info_fuente
{
	std::string nombre;
	int tam;
	bool operator<(const info_fuente& o) const
	{
		if(nombre < o.nombre) return false;
		else if(nombre > o.nombre) return true;
		else return tam < o.tam;
	}
};

class Fuentes
{
	public:

	const DLibV::Fuente_TTF&			obtener_fuente(const std::string, int) const;
	void						registrar_fuente(const std::string, int);

	private:

	std::map<info_fuente, DLibV::Fuente_TTF>	fuentes;
};

}

#endif
