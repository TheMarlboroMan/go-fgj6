#ifndef REPRESENTABLE_H
#define REPRESENTABLE_H

#include <video/pantalla/pantalla.h>
#include <video/camara/camara.h>

#include "representador.h"
#include "color.h"

namespace App
{

class Representable
{
	public:

	virtual void dibujar(Representador&, DLibV::Pantalla&, const DLibV::Camara&) const=0;
};

}

#endif
