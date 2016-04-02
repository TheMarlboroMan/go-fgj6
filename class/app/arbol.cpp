#include "arbol.h"

#include <video/representacion/representacion_grafica/representacion_bitmap/representacion_bitmap.h>
#include <video/gestores/gestor_texturas.h>

using namespace App;

Arbol::Arbol(Espaciable::tpunto pt)
{
	formar_poligono();
	establecer_posicion(pt.x, pt.y);
	for(int i=1; i<=5; ++i) piezas_colocadas[i]=false;
}

void Arbol::formar_poligono()
{
	poligono.insertar_vertice({-120.0, 120.0});
	poligono.insertar_vertice({120.0, 120.0});
	poligono.insertar_vertice({120.0, -120.0});
	poligono.insertar_vertice({-120.0, -120.0});
	poligono.cerrar();
	poligono.mut_centro({0.0, 0.0});
}

void Arbol::colocar_pieza(int p)
{
	piezas_colocadas[p]=true;
}

bool Arbol::es_finalizado() const
{
	size_t res=0;
	for(const auto& p: piezas_colocadas) res+=p.second ? 1 : 0;
	return res==piezas_colocadas.size();
}

void Arbol::dibujar(Representador& r, DLibV::Pantalla& pantalla, const DLibV::Camara& camara) const
{
	auto c=poligono.acc_centro();
	int x=c.x-80;
	int y=-c.y+160;

	for(const auto& p: piezas_colocadas)
	{
		DLibV::Representacion_bitmap bmp(DLibV::Gestor_texturas::obtener(4));
		DLibV::Representacion_bitmap flare(DLibV::Gestor_texturas::obtener(1));

		flare.establecer_modo_blend(DLibV::Representacion::BLEND_ALPHA);
		flare.establecer_alpha(255);
		flare.ir_a(x - 25, y - 25);
		flare.volcar(pantalla, camara);

		bmp.establecer_modo_blend(DLibV::Representacion::BLEND_ALPHA);
		bmp.establecer_alpha(p.second ? 255 : 64);
		bmp.establecer_recorte((p.first-1) * 30, 0, 30, 30);
		bmp.establecer_posicion(x - 15, y - 15, 30, 30);
		bmp.volcar(pantalla, camara);

		x+=50;
	}
}