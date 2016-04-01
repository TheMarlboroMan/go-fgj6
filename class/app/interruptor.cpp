#include "interruptor.h"

using namespace App;

Interruptor::Interruptor(Espaciable::tpunto pt, int n, int idp, int idg, int t)
	:nivel(n), id_puerta(idp), id_grupo(idg), tiempo_grupo(t) 
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
