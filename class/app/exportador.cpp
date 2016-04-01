#include "exportador.h"

using namespace App;

Herramientas_proyecto::Dnot_token::t_vector Exportador::generar_punto(double x, double y)
{
	using namespace Herramientas_proyecto;
	Dnot_token::t_vector arr;
	arr.push_back(Dnot_token(x));
	arr.push_back(Dnot_token(y));
	return arr;
}

Herramientas_proyecto::Dnot_token::t_vector Exportador::generar_color(int r, int g, int b, int a)
{
	using namespace Herramientas_proyecto;
	Dnot_token::t_vector arr;
	arr.push_back(Dnot_token(r));
	arr.push_back(Dnot_token(g));
	arr.push_back(Dnot_token(b));
	arr.push_back(Dnot_token(a));
	return arr;
}

Herramientas_proyecto::Dnot_token Exportador::serializar_obstaculo(const Obstaculo& o)
{
	using namespace Herramientas_proyecto;

	Dnot_token::t_vector puntos;
	for(const auto& v : o.acc_poligono().acc_vertices())
		puntos.push_back(Dnot_token(generar_punto(v.x, v.y)));

	auto centro=o.acc_poligono().acc_centro();

	Dnot_token::t_mapa propiedades;

	int tipo=0;
	switch(o.acc_tipo())
	{
		case Obstaculo::ttipos::normal: tipo=0; break;
		case Obstaculo::ttipos::inocuo: tipo=1; break;
	}

	propiedades["t"].asignar(tipo);
	
	Dnot_token::t_mapa mapa_objeto;
	mapa_objeto["p"].asignar(puntos);
	mapa_objeto["cen"].asignar(generar_punto(centro.x, centro.y));
	mapa_objeto["pr"].asignar(propiedades);

	return Dnot_token(mapa_objeto);
}

Herramientas_proyecto::Dnot_token Exportador::serializar_decoracion(const Decoracion& o)
{
	using namespace Herramientas_proyecto;

	Dnot_token::t_vector puntos;
	for(const auto& v : o.acc_poligono().acc_vertices())
		puntos.push_back(Dnot_token(generar_punto(v.x, v.y)));

	auto centro=o.acc_poligono().acc_centro();
	auto color=o.acc_color();
	auto colorl=o.acc_color_linea();

	Dnot_token::t_mapa mapa_polig;
	mapa_polig["p"].asignar(puntos);
	mapa_polig["cen"].asignar(generar_punto(centro.x, centro.y));

	auto ncolor=generar_color(color.r, color.g, color.b, color.a);
	auto ncolorl=generar_color(colorl.r, colorl.g, colorl.b, colorl.a);

	Dnot_token::t_vector arr_colores;
	arr_colores.push_back(Dnot_token(ncolor));
	arr_colores.push_back(Dnot_token(ncolorl));

	Dnot_token::t_mapa propiedades;
	propiedades["fr"].asignar(o.es_frente());
	propiedades["pr"].asignar(o.acc_profundidad());

	mapa_polig["col"].asignar(arr_colores);
	mapa_polig["pr"].asignar(propiedades);

	return Dnot_token(mapa_polig);
}

Herramientas_proyecto::Dnot_token Exportador::serializar_punto(DLibH::Punto_2d<double> pt)
{
	using namespace Herramientas_proyecto;

	Dnot_token::t_vector puntos;
	puntos.push_back(Dnot_token(pt.x));
	puntos.push_back(Dnot_token(pt.y));

	return Dnot_token(puntos);
}

std::string Exportador::serializar(const std::vector<Obstaculo>& obs,
		const std::vector<Decoracion>& decs)
{
	using namespace Herramientas_proyecto;

	Dnot_token::t_vector vobstaculos;
	for(const auto& o : obs) vobstaculos.push_back(serializar_obstaculo(o));

	Dnot_token::t_vector vdecoraciones;
	for(const auto& d : decs) vdecoraciones.push_back(serializar_decoracion(d));

	Dnot_token tok_obstaculos(vobstaculos);
	Dnot_token tok_decoraciones(vdecoraciones);

	Dnot_token::t_mapa mapa_final;
	mapa_final["geometria"]=tok_obstaculos;
	mapa_final["decoracion"]=tok_decoraciones;
	
	Dnot_token base(mapa_final);
	return base.serializar();
}
