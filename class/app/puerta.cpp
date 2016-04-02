#include "puerta.h"

using namespace App;

Puerta::Puerta(const tpoligono& p, int id)
	:Espaciable(p), id(id)
{

}

void Puerta::dibujar(Representador& r, DLibV::Pantalla& pantalla, const DLibV::Camara& camara) const
{
	r.dibujar_poligono(pantalla, poligono, {102, 51, 0, 255}, camara);
}
