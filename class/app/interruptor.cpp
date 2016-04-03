#include "interruptor.h"

#include <video/representacion/representacion_grafica/representacion_bitmap/representacion_bitmap.h>
#include <video/gestores/gestor_texturas.h>
#include <class/valor_limitado.h>

using namespace App;

Interruptor::Interruptor(Espaciable::tpunto pt, int n, int idp, int idg, int t)
	:nivel(n), id_puerta(idp), id_grupo(idg), tiempo_grupo(t),
	angulo(0.0f), tiempo_activo(0.0f, 0.0f, 0.0f, Herramientas_proyecto::Valor_limitado<float>::inferior)
{
	formar_poligono();
	establecer_posicion(pt.x, pt.y);
}

void Interruptor::formar_poligono()
{
	poligono.insertar_vertice({20.0, 30.0});
	poligono.insertar_vertice({30.0, 20.0});
	poligono.insertar_vertice({30.0, -20.0});
	poligono.insertar_vertice({20.0, -30.0});
	poligono.insertar_vertice({-20.0, -30.0});
	poligono.insertar_vertice({-30.0, -20.0});
	poligono.insertar_vertice({-30.0, 20.0});
	poligono.insertar_vertice({-20.0, 30.0});
	poligono.cerrar();
	poligono.mut_centro({0.0, 0.0});
}

void Interruptor::turno(float delta)
{
	if(tiempo_activo > 0.f)
	{
		tiempo_activo-=delta;
		angulo+=tiempo_activo * delta * 10.f;
		poligono.rotar(angulo);
	}
}

void Interruptor::activar()
{
	tiempo_activo=(float)tiempo_grupo/1000.f;
}


void Interruptor::dibujar(Representador& r, DLibV::Pantalla& pantalla, const DLibV::Camara& camara) const
{
	auto c=poligono.acc_centro();

	DLibV::Representacion_bitmap sprite(DLibV::Gestor_texturas::obtener(4));
	sprite.establecer_modo_blend(DLibV::Representacion::BLEND_ALPHA);
	sprite.establecer_alpha(255);
	sprite.establecer_recorte(0, 30, 60, 60);
	sprite.establecer_posicion(c.x-30, -c.y-30, 60, 60);
	//TODO...
	sprite.transformar_centro_rotacion(30 / camara.acc_zoom(), 30 / camara.acc_zoom());
	sprite.transformar_rotar(-angulo * 100.0);
	sprite.volcar(pantalla, camara);
}
