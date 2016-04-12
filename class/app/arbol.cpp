#include "arbol.h"

#include <video/representacion/representacion_grafica/representacion_bitmap/representacion_bitmap.h>
#include <video/gestores/gestor_texturas.h>

#include "recursos.h"

using namespace App;

Arbol::Arbol(Espaciable::tpunto pt)
	:tiempo_florecimiento(0.0)
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
		DLibV::Representacion_bitmap bmp(DLibV::Gestor_texturas::obtener(r_graficos::g_sprites));
		DLibV::Representacion_bitmap flare(DLibV::Gestor_texturas::obtener(r_graficos::g_lens_flare));

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

	if(tiempo_florecimiento)
	{
		int alpha=(tiempo_florecimiento * 254.0) / 2.5f;
		if(alpha > 255) alpha=255;

		DLibV::Representacion_bitmap flores(DLibV::Gestor_texturas::obtener(r_graficos::g_flores));
		flores.establecer_modo_blend(DLibV::Representacion::BLEND_ALPHA);
		flores.establecer_alpha(alpha);
		flores.establecer_posicion(c.x-240, -c.y-270);
		flores.volcar(pantalla, camara);
	}
}

void Arbol::turno(float delta)
{
	tiempo_florecimiento+=delta;
	if(tiempo_florecimiento > 5.0f) tiempo_florecimiento=5.0f;
}
