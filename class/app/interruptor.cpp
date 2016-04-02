#include "interruptor.h"

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
	poligono.insertar_vertice({10.0, 20.0});
	poligono.insertar_vertice({20.0, 10.0});
	poligono.insertar_vertice({20.0, -10.0});
	poligono.insertar_vertice({10.0, -20.0});
	poligono.insertar_vertice({-10.0, -20.0});
	poligono.insertar_vertice({-20.0, -10.0});
	poligono.insertar_vertice({-20.0, 10.0});
	poligono.insertar_vertice({-10.0, 20.0});
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
	r.dibujar_poligono(pantalla, poligono, {102, 153, 255, 255}, camara);
}
